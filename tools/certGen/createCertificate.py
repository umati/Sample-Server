#!/usr/bin/env python3
import argparse
import json
import os
import shutil
from pathlib import Path
from typing import Any, Dict

from jinja2 import Template


class Settings:
    def __init__(self, settings):
        self._defaults = {
            "Hostnames": ["localhost"],
            "IPs": [],
            "URI": "urn:UmatiSampleServer",
            "Keysize": 2048,
            "Days": 365,
            "Subject": "/C=DE/O=SampleOrganization/CN=UmatiSampleServer@localhost/L=Frankfurt am Main",
        }
        self._settings = settings

    def settings(self):
        return {**self._defaults, **self._settings}


def writeConfiguration(src: Path, dst: Path, settings: Settings):
    if not src.suffix == ".jinja2":
        shutil.copyfile(src, dst, follow_symlinks=True)
        return
    resOld = ""
    res = src.read_text()
    while resOld != res:
        resOld = res
        res = Template(res).render(settings.settings())
    with dst.open("w") as file:
        file.write(res)


basePath = Path(os.path.realpath(__file__)).parent

data: Dict[str, Any] = {}

# defaultSettings = basePath / 'defaultSettings.json'
# with open(defaultSettings) as f:
#     data = json.load(f)
#     print(data)

parser = argparse.ArgumentParser(description="Create configuration files")
parser.add_argument("enviroment", help="The enviroment to be create.")
parser.add_argument(
    "outDir", nargs="?", help="Output directory. Default: enviroment name."
)
args = parser.parse_args()

envSettings = Path(f"{args.enviroment}.json")
if envSettings.exists():
    with open(envSettings) as f:
        data = {**data, **json.load(f)}

settings = Settings(data)
print(settings.settings())

templateOpensslCnf = basePath / "openssl.cnf.jinja2"
targetDir = Path(args.outDir if args.outDir is not None else args.enviroment)
if not targetDir.exists():
    os.mkdir(targetDir)

targetOpensslCnf = targetDir / "openssl.cnf"
writeConfiguration(templateOpensslCnf, targetOpensslCnf, settings)

outCrt = targetDir / f"{args.enviroment}.crt"
outKey = targetDir / f"{args.enviroment}.key"
outCert = targetDir / "server_cert.der"
outKeyDer = targetDir / "server_key.der"

os.system(
    f"openssl req \
     -config \"{targetOpensslCnf}\" \
     -new \
     -nodes \
     -x509 -sha256  \
     -newkey rsa:{settings.settings()['Keysize']} \
     -keyout \"{outKey}\" -days {settings.settings()['Days']} \
     -subj \"{settings.settings()['Subject']}\"\
     -out \"{outCrt}\""
)
os.system(f'openssl x509 -in "{outCrt}" -outform der -out "{outCert}"')
os.system(f'openssl rsa -inform PEM -in "{outKey}" -outform DER -out "{outKeyDer}"')

print(f"Use the generated {outCert} and {outKeyDer} for the server.")

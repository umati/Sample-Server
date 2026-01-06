# Certificate generator

This is a basic self sign certificate generator for an OPC UA Server.

## Usage

- Create a Python virtual enviroment using `python3 -m venv .venv` and activate it with `source .venv/bin/activate`.

- Install the required packages `pip3 install -r requirements.txt`.

- Make a copy of `settings.template.json` and name it `<env>.json` (`<env>` is the name of your enviroment). Adapt your hostname(s) and IP adresses.

- Call `python3 createCertificate.py <env>`.

- All certificates are stored created in a folder named `<env>`. Copy `server_key.der` and `server_cert.der` in the directory of the server executable.

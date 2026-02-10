#!/bin/bash

# Get the current directory
REPO_DIR=$(pwd)
echo "$REPO_DIR"

docker pull ghcr.io/super-linter/super-linter:v8.4.0

# Run the Docker container with the specified environment variables and volume mount
docker run \
	-e IGNORE_GITIGNORED_FILES=true \
	-e LOG_LEVEL=INFO \
	-e DEFAULT_BRANCH=origin/develop \
	-e VALIDATE_ALL_CODEBASE=true \
	-e RUN_LOCAL=true \
	-e VALIDATE_CPP=false \
	-e VALIDATE_ANSIBLE=false \
    -e VALIDATE_BIOME_FORMAT=false \
    -e VALIDATE_BIOME_LINT=false \
	-e VALIDATE_PYTHON_RUFF=false \
	-e VALIDATE_PYTHON_RUFF_FORMAT=false \
	-e VALIDATE_GIT_COMMITLINT=false \
	-e FIX_PYTHON_BLACK=true \
	-e FIX_SPELL_CODESPELL=true \
	-v "$REPO_DIR:/tmp/lint" -it --rm ghcr.io/super-linter/super-linter:v8.4.0


	# -e VALIDATE_CLANG_FORMAT=true \
	# -e VALIDATE_PYTHON_RUFF_FORMAT=true \
	# -e FIX_CLANG_FORMAT=true \
	# -e FIX_PYTHON_RUFF_FORMAT=true \
#	-e FILTER_REGEX_EXCLUDE="deps/*" \
# -e VALIDATE_ANSIBLE=false \
# 	-e VALIDATE_ARM=false \
# 	-e VALIDATE_BASH_EXEC=false \
# 	-e VALIDATE_CPP=false \
# 	-e VALIDATE_CHECKOV=false \
# 	-e VALIDATE_CLANG_FORMAT=false \
# 	-e VALIDATE_CLOJURE=false \
# 	-e VALIDATE_CLOUDFORMATION=false \
# 	-e VALIDATE_COFFEESCRIPT=false \
# 	-e VALIDATE_CSHARP=false \
# 	-e VALIDATE_CSS=false \
# 	-e VALIDATE_DART=false \
# 	-e VALIDATE_DOCKERFILE_HADOLINT=false \
# 	-e VALIDATE_EDITORCONFIG=false \
# 	-e VALIDATE_ENV=false \
# 	-e VALIDATE_GITHUB_ACTIONS=false \
# 	-e VALIDATE_GITHUB_ACTIONS_ZIZMOR=false \
# 	-e VALIDATE_GITLEAKS=false \
# 	-e VALIDATE_GO=false \
# 	-e VALIDATE_GO_MODULES=false \
# 	-e VALIDATE_GO_RELEASER=false \
# 	-e VALIDATE_GOOGLE_JAVA_FORMAT=false \
# 	-e VALIDATE_GROOVY=false \
# 	-e VALIDATE_HTML=false \
# 	-e VALIDATE_JAVA=false \
# 	-e VALIDATE_JSCPD=false \
# 	-e VALIDATE_JSON=false \
# 	-e VALIDATE_JSX=false \
# 	-e VALIDATE_KOTLIN=false \
# 	-e VALIDATE_LATEX=false \
# 	-e VALIDATE_LUA=false \
# 	-e VALIDATE_MARKDOWN=false \
# 	-e VALIDATE_NATURAL_LANGUAGE=false \
# 	-e VALIDATE_OPENAPI=false \
# 	-e VALIDATE_PERL=false \
# 	-e VALIDATE_PHP=false \
# 	-e VALIDATE_PHP_BUILTIN=false \
# 	-e VALIDATE_PHP_PHPCS=false \
# 	-e VALIDATE_PHP_PHPSTAN=false \
# 	-e VALIDATE_PHP_PSALM=false \
# 	-e VALIDATE_POWERSHELL=false \
# 	-e VALIDATE_PROTOBUF=false \
# 	-e VALIDATE_PYTHON=false \
# 	-e VALIDATE_PYTHON_BLACK=false \
# 	-e VALIDATE_PYTHON_FLAKE8=false \
# 	-e VALIDATE_PYTHON_ISORT=false \
# 	-e VALIDATE_PYTHON_MYPY=false \
# 	-e VALIDATE_PYTHON_PYLINT=false \
# 	-e VALIDATE_PYTHON_RUFF=false \
# 	-e VALIDATE_R=false \
# 	-e VALIDATE_RENOVATE=false \
# 	-e VALIDATE_RUBY=false \
# 	-e VALIDATE_RUST_2015=false \
# 	-e VALIDATE_RUST_2018=false \
# 	-e VALIDATE_RUST_2021=false \
# 	-e VALIDATE_RUST_CLIPPY=false \
# 	-e VALIDATE_SCALAFMT=false \
# 	-e VALIDATE_SHELL_SHFMT=false \
# 	-e VALIDATE_SNAKEMAKE_LINT=false \
# 	-e VALIDATE_SNAKEMAKE_SNAKEFMT=false \
# 	-e VALIDATE_STATES=false \
# 	-e VALIDATE_SQLFLUFF=false \
# 	-e VALIDATE_TERRAFORM_FMT=false \
# 	-e VALIDATE_TERRAFORM_TERRASCAN=false \
# 	-e VALIDATE_TERRAFORM_TFLINT=false \
# 	-e VALIDATE_TERRAGRUNT=false \
# 	-e VALIDATE_TRIVY=false \
# 	-e VALIDATE_XML=false \
# 	-e VALIDATE_YAML=false \
# 	-e VALIDATE_HTML_PRETTIER=false \
# 	-e VALIDATE_JAVASCRIPT_PRETTIER=false \
# 	-e VALIDATE_JAVASCRIPT_ES=false \
# 	-e VALIDATE_JSON_PRETTIER=false \
# 	-e VALIDATE_JSONC=false \
# 	-e VALIDATE_JSONC_PRETTIER=false \
# 	-e VALIDATE_GRAPHQL_PRETTIER=false \
# 	-e VALIDATE_JSX_PRETTIER=false \
# 	-e VALIDATE_MARKDOWN_PRETTIER=false \
# 	-e VALIDATE_TYPESCRIPT_PRETTIER=false \
# 	-e VALIDATE_TYPESCRIPT_ES=false \
# 	-e VALIDATE_TSX=false \
# 	-e VALIDATE_YAML_PRETTIER=false \
# 	-e VALIDATE_CSS_PRETTIER=false \
# 	-e VALIDATE_VUE_PRETTIER=false \
# 	-e VALIDATE_VUE=false \
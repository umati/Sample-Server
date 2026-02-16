# MIT License
# 
# Copyright (c) 2019-2021  Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
# Copyright (c) 2021-2023  Götz Görisch, VDW - Verein Deutscher Werkzeugmaschinenfabriken e.V.
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
FROM alpine:3.23.3 AS build-env

RUN apk --no-cache add \
      bash=5.3.3-r1 \
      cmake=4.1.3-r0 \
      gcc=15.2.0-r2 \
      g++=15.2.0-r2 \
      git=2.52.0-r0 \
      make=4.4.1-r3  \
      python3=3.12.12-r0 \
      patch=2.8-r0 \
      linux-headers=6.16.12-r0 && \
    mkdir /install

ARG BUILD_TYPE=Debug

COPY . /src/Sample-Server

WORKDIR /build
RUN cmake /src/Sample-Server/.github/ \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX:PATH=/install /build && \
    cmake --build . --parallel "$(nproc)"

FROM alpine:3.23.3 AS runtime
RUN apk --no-cache add \
      libstdc++=15.2.0-r2
COPY --from=build-env /install/bin /app
COPY ./configuration.docker.json /configuration.json

RUN adduser -DH sampleserver
USER sampleserver

EXPOSE 4840

ENTRYPOINT ["/app/SampleServer"]

HEALTHCHECK CMD netstat -ltn | grep -c 4840

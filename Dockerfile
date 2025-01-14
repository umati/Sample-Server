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
FROM alpine:3.21.2 AS build-env

RUN apk --no-cache add \
      bash=5.2.37-r0 \
      cmake=3.31.1-r0 \
      gcc=14.2.0-r4 \
      g++=14.2.0-r4 \
      git=2.47.1-r0 \
      make=4.4.1-r2  \
      python3=3.12.8-r1 \
      patch=2.7.6-r10 \
      linux-headers=6.6-r1 && \
    mkdir /install

ARG BUILD_TYPE=Debug

COPY . /src/Sample-Server

WORKDIR /build
RUN cmake /src/Sample-Server/.github/ \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX:PATH=/install /build && \
    cmake --build .

FROM alpine:3.21.2 AS runtime
RUN apk --no-cache add \
      libstdc++=14.2.0-r4
COPY --from=build-env /install/bin /app
COPY ./configuration.docker.json /configuration.json

RUN adduser -DH sampleserver
USER sampleserver

EXPOSE 4840

ENTRYPOINT ["/app/SampleServer"]

HEALTHCHECK CMD netstat -ltn | grep -c 4840

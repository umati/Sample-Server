FROM alpine:3.15.0 as build-env

RUN apk --no-cache add \
      bash=5.1.8-r0\
      cmake=3.21.3-r0 \
      gcc=10.3.1_git20211027-r0 \
      g++=10.3.1_git20211027-r0 \
      git=2.34.1-r0 \
      make=4.3-r0  \
      python3=3.9.7-r4 \
      patch=2.7.6-r7 && \
    mkdir /install

ARG BUILD_TYPE=Debug

COPY . /src/Sample-Server

WORKDIR /build
RUN cmake /src/Sample-Server/.github/ \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX:PATH=/install /build &&\
    cmake --build .

FROM alpine:3.15.0 as runtime
RUN apk --no-cache add \
      libstdc++=10.3.1_git20211027-r0
COPY --from=build-env /install/bin /app
COPY ./configuration.docker.json /configuration.json

EXPOSE 4840

ENTRYPOINT ["/app/SampleServer"]

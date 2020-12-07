FROM alpine:latest as build-env

RUN apk update && \
    apk upgrade && \
    apk --update add \
        gcc \
        g++ \
        cmake \
        bash \
        git \
        make \
        python3 \
		&& \
    rm -rf /var/cache/apk/*

RUN mkdir /install

ARG BUILD_TYPE=Debug

COPY Sample-Server /src/Sample-Server

RUN mkdir -p /build && \
    cd /build && \
    cmake /src/Sample-Server/.github/ \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX:PATH=/install &&\
    cmake --build .

FROM alpine:latest as runtime
RUN apk update && \
    apk upgrade && \
    apk --update add \
      libstdc++ && \
    rm -rf /var/cache/apk/*
COPY --from=build-env /install/bin /app

EXPOSE 4840

ENTRYPOINT ["/app/SampleServer"]

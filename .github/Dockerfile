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

RUN mkdir -p /build/googletest && \
    cd /build/googletest && \
    cmake /src/Sample-Server/deps/googletest \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX:PATH=/install &&\
    cmake --build . --target install

RUN mkdir -p /build/mbedtls && \
    cd /build/mbedtls && \
    cmake /src/Sample-Server/deps/mbedtls \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX:PATH=/install \
      -DENABLE_PROGRAMS:BOOL=0 \
      -DENABLE_TESTING:BOOL=0 \
		&&\
    cmake --build . --target install

RUN mkdir -p /build/open62541 && \
    cd /build/open62541 && \
    cmake /src/Sample-Server/deps/open62541 \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DUA_LOGLEVEL=400 \
      -DUA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS:BOOL=ON \
      -DUA_ENABLE_SUBSCRIPTIONS_EVENTS:BOOL=ON \
      -DUA_NAMESPACE_ZERO:STRING=FULL \
		  -DUA_ENABLE_ENCRYPTION:BOOL=1 \
		  -DUA_ENABLE_ENCRYPTION_MBEDTLS:BOOL=1 \
        -DCMAKE_INSTALL_PREFIX:PATH=/install && \
    cmake --build . --target install

RUN mkdir -p /build/open62541Cpp && \
    cd /build/open62541Cpp && \
    cmake /src/Sample-Server/deps/open62541Cpp \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX:PATH=/install && \
    cmake --build . --target install

RUN mkdir -p /build/Sample-Server && \
    cd /build/Sample-Server && \
    cmake /src/Sample-Server \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX:PATH=/install && \
    cmake --build . --target install


FROM alpine:latest as runtime
RUN apk update && \
    apk upgrade && \
    apk --update add \
      libstdc++ && \
    rm -rf /var/cache/apk/*
COPY --from=build-env /install/bin /app

EXPOSE 4840

ENTRYPOINT ["/app/SampleServer"]

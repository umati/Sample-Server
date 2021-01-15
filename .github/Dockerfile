FROM alpine:3 as build-env

RUN apk --no-cache add \
      bash=~5.1.0 \
      cmake=~3.18.4 \
      gcc=~10.2.1 \
      g++=~10.2.1 \
      git=~2.30.0\
      make=~4.3 \
      python3=~3.8.7

RUN mkdir /install

ARG BUILD_TYPE=Debug

COPY Sample-Server /src/Sample-Server

WORKDIR /build
RUN cmake /src/Sample-Server/.github/ \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX:PATH=/install /build &&\
    cmake --build .

FROM alpine:3 as runtime
RUN apk --no-cache add \
      libstdc++=~10.2.1
COPY --from=build-env /install/bin /app

EXPOSE 4840

ENTRYPOINT ["/app/SampleServer"]

FROM alpine:3 as build-env

RUN apk --no-cache add \
      bash=~5.1.4 \
      cmake=~3.20.3 \
      gcc=~10.3.1 \
      g++=~10.3.1 \
      git=~2.32\
      make=~4.3 \
      python3=~3.9.5 \
      patch=~2.7.6-r7 && \
    mkdir /install

ARG BUILD_TYPE=Debug

COPY . /src/Sample-Server

WORKDIR /build
RUN cmake /src/Sample-Server/.github/ \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX:PATH=/install /build &&\
    cmake --build .

FROM alpine:3 as runtime
RUN apk --no-cache add \
      libstdc++=~10.3.1
COPY --from=build-env /install/bin /app
COPY ./configuration.docker.json /configuration.json

EXPOSE 4840

ENTRYPOINT ["/app/SampleServer"]

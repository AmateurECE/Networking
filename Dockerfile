FROM alpine:latest

RUN apk add build-base openssl openssl-dev cmake gtest-dev

WORKDIR /root
ENTRYPOINT ["openssl"]

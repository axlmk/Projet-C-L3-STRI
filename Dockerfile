FROM alpine:latest
COPY . /app
RUN apk add gcc make libc-dev
RUN cd /app/client/sources && make
RUN cd /app/server/sources && make
ENTRYPOINT ["/bin/sh"]

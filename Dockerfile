# Stage builder
FROM alpine:edge as builder

# Create working directory
RUN mkdir -p /usr/app
WORKDIR /usr/app

# Copy codebase into docker container
COPY . /usr/app

# Include packages from the testing repository of the edge branch
RUN echo "http://dl-4.alpinelinux.org/alpine/edge/testing" >> /etc/apk/repositories

# Install necessary dependencies
RUN apk --no-cache add --virtual .build-deps g++ make cmake lcov

# Build libraries
RUN mkdir -p build && cd build && cmake ../src && make -j

# Stage deployer
FROM alpine:latest as deployer

# Create working directory
RUN mkdir -p /usr/app
WORKDIR /usr/app

# Copy bayesian-estimation libraries
COPY --from=builder /usr/app/build/lib/*  /usr/local/lib/

# Copy all the source code, but we need only the headers
COPY --from=builder /usr/app/src /usr/app/

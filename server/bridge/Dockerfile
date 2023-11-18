# syntax=docker/dockerfile:1

ARG GCC_VERSION=13.2.0
FROM gcc:$GCC_VERSION as preface

ARG CMAKE_VERSION=3.21.0

RUN wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-Linux-x86_64.sh \
      -q -O /tmp/cmake-install.sh \
      && chmod u+x /tmp/cmake-install.sh \
      && mkdir /usr/bin/cmake \
      && /tmp/cmake-install.sh --skip-license --prefix=/usr/bin/cmake \
      && rm /tmp/cmake-install.sh

ENV PATH="/usr/bin/cmake/bin:${PATH}"
ENV CGO_ENABLED=0

COPY craft-rank/ .

RUN mkdir build && cd build && cmake .. && cmake --build . && cmake --install . && make && cd ..

# Comments are provided throughout this file to help you get started.
# If you need more help, visit the Dockerfile reference guide at
# https://docs.docker.com/engine/reference/builder/


FROM node:20.9.0-bookworm
COPY --from=preface build/CraftRank /usr/src/app/src/CraftRank

ENV PATH="/usr/src/app/src:${PATH}"
RUN chmod 777 /usr/src/app/src/CraftRank

WORKDIR /usr/src/app

# Use production node environment by default.
ENV NODE_ENV production

# RUN apk update
# RUN apk upgrade
# RUN apk add coreutils gcc g++
# RUN apk add --no-cache bash
# RUN apk add --no-cache libc6-compat 

# Download dependencies as a separate step to take advantage of Docker's caching.
# Leverage a cache mount to /root/.npm to speed up subsequent builds.
# Leverage a bind mounts to package.json and package-lock.json to avoid having to copy them into
# into this layer.
RUN --mount=type=bind,source=package.json,target=package.json \
    --mount=type=bind,source=package-lock.json,target=package-lock.json \
    --mount=type=cache,target=/root/.npm \
    npm ci --omit=dev

# Run the application as a non-root user.
USER node

# Copy the rest of the source files into the image.
COPY . .

# Expose the port that the application listens on.
EXPOSE 3000

# I HATE DOCKER WITH A BURNING PASSION

# Run the application.
CMD node src/index.js

ARG GCC_VERSION=13.2.0
FROM gcc:$GCC_VERSION

ARG CMAKE_VERSION=3.21.0

RUN wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-Linux-x86_64.sh \
      -q -O /tmp/cmake-install.sh \
      && chmod u+x /tmp/cmake-install.sh \
      && mkdir /usr/bin/cmake \
      && /tmp/cmake-install.sh --skip-license --prefix=/usr/bin/cmake \
      && rm /tmp/cmake-install.sh

ENV PATH="/usr/bin/cmake/bin:${PATH}"

COPY . .

RUN mkdir build
RUN cd build
RUN cmake ..
RUN cmake --build .
RUN cmake --install .
RUN make
RUN cd ..

CMD ./CraftRank
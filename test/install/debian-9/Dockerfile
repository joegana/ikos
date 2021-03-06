FROM debian:9
MAINTAINER Maxime Arthaud <maxime.arthaud@nasa.gov>
ARG njobs=2
ARG build_type=Release

# Installs the following versions (note that it might be out of date):
# cmake 3.7.2
# gmp 6.1.2
# boost 1.62.0
# python 2.7.13
# sqlite 3.16.2
# llvm 4.0.1
# clang 4.0.1
# gcc 6.3.0

# Upgrade
RUN apt-get update
RUN apt-get upgrade -y

# Install all dependencies
RUN apt-get install -y gcc g++ cmake libgmp-dev libboost-dev libboost-filesystem-dev \
        libboost-test-dev python python-pygments libsqlite3-dev libz-dev libedit-dev \
        llvm-4.0 llvm-4.0-dev clang-4.0

# Update path to include llvm
ENV PATH "/usr/lib/llvm-4.0/bin:$PATH"

# Add ikos source code
ADD . /root/ikos

# Build ikos
RUN rm -rf /root/ikos/build && mkdir /root/ikos/build
WORKDIR /root/ikos/build
ENV MAKEFLAGS "-j$njobs"
RUN cmake \
        -DCMAKE_INSTALL_PREFIX="/opt/ikos" \
        -DCMAKE_BUILD_TYPE="$build_type" \
        ..
RUN make
RUN make install

# Run the tests
RUN make check

# Add ikos to the path
ENV PATH "/opt/ikos/bin:$PATH"

# Done
WORKDIR /

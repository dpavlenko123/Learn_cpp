FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ARG LLVM_VERSION=20
ARG USERNAME=student
ARG USER_UID=1000
ARG USER_GID=1000

# ===== Base packages =====
RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    wget \
    curl \
    git \
    openssh-client \
    sudo \
    gnupg \
    lsb-release \
    software-properties-common \
    ninja-build \
    pkg-config \
    tar \
    xz-utils \
    python3 \
    python3-pip \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/*

# ===== LLVM repository =====
RUN wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key \
    | gpg --dearmor \
    > /usr/share/keyrings/llvm.gpg \
    && echo "deb [signed-by=/usr/share/keyrings/llvm.gpg] http://apt.llvm.org/jammy/ llvm-toolchain-jammy-${LLVM_VERSION} main" \
    > /etc/apt/sources.list.d/llvm.list

# ===== Install LLVM / Clang =====
RUN apt-get update && apt-get install -y --no-install-recommends \
    clang-${LLVM_VERSION} \
    clangd-${LLVM_VERSION} \
    clang-tidy-${LLVM_VERSION} \
    clang-format-${LLVM_VERSION} \
    lldb-${LLVM_VERSION} \
    lld-${LLVM_VERSION} \
    llvm-${LLVM_VERSION} \
    llvm-${LLVM_VERSION}-dev \
    libc++-${LLVM_VERSION}-dev \
    libclang-rt-${LLVM_VERSION}-dev \
    libc++abi-${LLVM_VERSION}-dev \
    libunwind-${LLVM_VERSION}-dev \
    libomp-${LLVM_VERSION}-dev \
    && rm -rf /var/lib/apt/lists/*

# ===== Update alternatives =====
RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-${LLVM_VERSION} 100 \
 && update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-${LLVM_VERSION} 100 \
 && update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-${LLVM_VERSION} 100 \
 && update-alternatives --install /usr/bin/lldb lldb /usr/bin/lldb-${LLVM_VERSION} 100

ENV CC=clang
ENV CXX=clang++

# ===== Install CMake (Kitware) =====
RUN wget -qO- https://apt.kitware.com/keys/kitware-archive-latest.asc \
    | gpg --dearmor \
    > /usr/share/keyrings/kitware.gpg \
    && echo "deb [signed-by=/usr/share/keyrings/kitware.gpg] https://apt.kitware.com/ubuntu/ jammy main" \
    > /etc/apt/sources.list.d/kitware.list \
    && apt-get update \
    && apt-get install -y cmake \
    && rm -rf /var/lib/apt/lists/*

# ===== Build GoogleTest =====
RUN git clone --depth 1 --branch v1.14.0 https://github.com/google/googletest.git \
    && cmake -S googletest -B googletest/build -G Ninja \
        -DCMAKE_CXX_COMPILER=clang++ \
        -DCMAKE_C_COMPILER=clang \
        -DCMAKE_CXX_FLAGS="-stdlib=libc++" \
        -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld -lc++ -lc++abi" \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
    && cmake --build googletest/build \
    && cmake --install googletest/build \
    && rm -rf googletest

# ===== Create restricted user =====
RUN groupadd --gid ${USER_GID} ${USERNAME} \
    && useradd --uid ${USER_UID} --gid ${USER_GID} -m ${USERNAME} \
    && echo "${USERNAME} ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/${USERNAME}

USER ${USERNAME}
WORKDIR /home/${USERNAME}/projects

CMD ["/bin/bash"]

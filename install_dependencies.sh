#!/bin/bash

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to run as root
run_as_root() {
    if [ "$(id -u)" != "0" ]; then
        echo "This script must be run as root. Trying with sudo..."
        sudo "$0" "$@"
        exit $?
    fi
}

# Call run_as_root at the beginning of the script
run_as_root "$@"

# Function to install packages
install_package() {
    if [ "$PKG_MANAGER" = "apt-get" ]; then
        apt-get install -y "$1"
    elif [ "$PKG_MANAGER" = "yum" ]; then
        yum install -y "$1"
    elif [ "$PKG_MANAGER" = "pacman" ]; then
        pacman -S --noconfirm "$1"
    elif [ "$PKG_MANAGER" = "zypper" ]; then
        zypper install -y "$1"
    fi
}

# Check if we're root
if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

# Detect the package manager
if command_exists apt-get; then
    PKG_MANAGER="apt-get"
    READLINE_PKG="libreadline-dev"
elif command_exists yum; then
    PKG_MANAGER="yum"
    READLINE_PKG="readline-devel"
elif command_exists pacman; then
    PKG_MANAGER="pacman"
    READLINE_PKG="readline"
elif command_exists zypper; then
    PKG_MANAGER="zypper"
    READLINE_PKG="readline-devel"
else
    echo "Unsupported package manager. Please install readline development package manually."
    exit 1
fi

# Update package lists
echo "Updating package lists..."
if [ "$PKG_MANAGER" = "apt-get" ]; then
    apt-get update
elif [ "$PKG_MANAGER" = "pacman" ]; then
    pacman -Sy
elif [ "$PKG_MANAGER" = "zypper" ]; then
    zypper refresh
fi

# Install readline development package
echo "Installing readline development package..."
install_package "$READLINE_PKG"

# Check if installation was successful
if command_exists pkg-config && pkg-config --exists readline; then
    echo "Readline development package installed successfully."
else
    echo "Failed to install readline development package. Please install it manually."
    exit 1
fi

# Add any other dependencies here
# For example:
# install_package "libssl-dev"
# install_package "libcurl4-openssl-dev"

echo "All dependencies installed successfully."


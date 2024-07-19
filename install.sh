#!/bin/bash

echo "This script will install necessary dependencies. You may be prompted for your password."

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

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
    sudo apt-get update
elif [ "$PKG_MANAGER" = "pacman" ]; then
    sudo pacman -Sy
elif [ "$PKG_MANAGER" = "zypper" ]; then
    sudo zypper refresh
fi

# Install readline development package
echo "Installing readline development package..."
if [ "$PKG_MANAGER" = "apt-get" ]; then
    sudo apt-get install -y "$READLINE_PKG"
elif [ "$PKG_MANAGER" = "yum" ]; then
    sudo yum install -y "$READLINE_PKG"
elif [ "$PKG_MANAGER" = "pacman" ]; then
    sudo pacman -S --noconfirm "$READLINE_PKG"
elif [ "$PKG_MANAGER" = "zypper" ]; then
    sudo zypper install -y "$READLINE_PKG"
fi

# Check if installation was successful
if command_exists pkg-config && pkg-config --exists readline; then
    echo "Readline development package installed successfully."
else
    echo "Failed to install readline development package. Please install it manually."
    exit 1
fi

echo "All dependencies installed successfully."

# Build the project
echo "Building the project..."
make clean
make

if [ $? -eq 0 ]; then
    echo "Build completed successfully."
else
    echo "Build failed. Please check the error messages above."
    exit 1
fi

echo "Installation and build process completed."

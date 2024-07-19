#!/bin/bash

echo "This shell file will check dependencies, install if necessary, and build ORORA."

command_exists() {
    command -v "$1" >/dev/null 2>&1
}

readline_installed() {
    pkg-config --exists readline
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
    echo "Unsupported package manager. Please install readline."
    exit 1
fi

if readline_installed; then
    echo "Readline development package is already installed."
else
    echo "Readline development package is not installed. Installing..."

    echo "Updating package lists..."
    if [ "$PKG_MANAGER" = "apt-get" ]; then
        sudo apt-get update
    elif [ "$PKG_MANAGER" = "pacman" ]; then
        sudo pacman -Sy
    elif [ "$PKG_MANAGER" = "zypper" ]; then
        sudo zypper refresh
    fi

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

    if readline_installed; then
        echo "Readline development package installed successfully."
    else
        echo "Failed to install readline development package."
        exit 1
    fi
fi

echo "Building the project..."
make clean
make

if [ $? -eq 0 ]; then
    echo "Build completed successfully."
else
    echo "Build failed. Please check the error messages above."
    exit 1
fi

# Install orora system-wide
echo "Installing orora system-wide..."
sudo mv build/orora /usr/local/bin/
if [ $? -ne 0 ]; then
    echo "Failed to install orora. Please check your permissions."
    exit 1
fi

echo "orora has been installed successfully."
echo "You can now run 'orora' from any directory."

# Optional: Set up man page or help documentation
# sudo cp orora.1 /usr/local/share/man/man1/
# sudo mandb

echo "Installation process completed."

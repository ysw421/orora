#!/bin/bash

check_sudo() {
    if sudo -n true 2>/dev/null; then
        echo "Sudo access is available."
    else
        echo "Sudo access is required for this script."
        echo "Please enter your password when prompted."
        if ! sudo -v; then
            echo "Failed to obtain sudo privileges. Exiting."
            exit 1
        fi
    fi
}

check_sudo
sudo chmod -R 755 build
sudo echo "This shell file will check dependencies, install if necessary, and build ORORA."

command_exists() {
    command -v "$1" >/dev/null 2>&1
}

readline_installed() {
    pkg-config --exists readline
}

libwebsockets_installed() {
    pkg-config --exists libwebsockets
}

# Detect the package manager
if command_exists apt-get; then
    PKG_MANAGER="apt-get"
    READLINE_PKG="libreadline-dev"
    LIBWEBSOCKETS_PKG="libwebsockets-dev"
elif command_exists yum; then
    PKG_MANAGER="yum"
    READLINE_PKG="readline-devel"
    LIBWEBSOCKETS_PKG="libwebsockets-devel"
elif command_exists pacman; then
    PKG_MANAGER="pacman"
    READLINE_PKG="readline"
    LIBWEBSOCKETS_PKG="libwebsockets"
elif command_exists zypper; then
    PKG_MANAGER="zypper"
    READLINE_PKG="readline-devel"
    LIBWEBSOCKETS_PKG="libwebsockets-devel"
else
    echo "Unsupported package manager. Please install readline and libwebsockets manually."
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

if libwebsockets_installed; then
    echo "libwebsockets development package is already installed."
else
    echo "libwebsockets development package is not installed. Installing..."
    echo "Installing libwebsockets development package..."
    if [ "$PKG_MANAGER" = "apt-get" ]; then
        sudo apt-get install -y "$LIBWEBSOCKETS_PKG"
    elif [ "$PKG_MANAGER" = "yum" ]; then
        sudo yum install -y "$LIBWEBSOCKETS_PKG"
    elif [ "$PKG_MANAGER" = "pacman" ]; then
        sudo pacman -S --noconfirm "$LIBWEBSOCKETS_PKG"
    elif [ "$PKG_MANAGER" = "zypper" ]; then
        sudo zypper install -y "$LIBWEBSOCKETS_PKG"
    fi
    if libwebsockets_installed; then
        echo "libwebsockets development package installed successfully."
    else
        echo "Failed to install libwebsockets development package."
        exit 1
    fi
fi

echo "Building the project..."
sudo make clean
sudo make uninstall
sudo make install
if [ $? -eq 0 ]; then
    echo "Build completed successfully."
else
    echo "Build failed. Please check the error messages above."
    exit 1
fi

echo "Installing orora system-wide..."
ORORA_INSTALL_DIR="/usr/share/orora"
ORORA_BIN="/usr/bin/orora"

if [ $? -ne 0 ]; then
    echo "Failed to install orora. Please check your permissions."
    exit 1
fi

echo "orora has been installed successfully!"
echo "You can now run 'orora' from any directory."

# sudo cp orora.1 /usr/local/share/man/man1/
# sudo mandb

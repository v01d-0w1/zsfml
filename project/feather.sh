#!/bin/bash

# SFML Compilation Script for Void Linux
# Usage: ./feather.sh [clean|rebuild|run]

PROJECT_NAME="SFMLProject"
BUILD_DIR="build"
ASSETS_DIR="assests"
PROGRAM_DIR="program"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}SFML Project Builder${NC}"

case "$1" in
    "clean")
        echo -e "${YELLOW}Cleaning build...${NC}"
        rm -rf $BUILD_DIR
        rm -rf $PROGRAM_DIR/$PROJECT_NAME
        rm -rf $PROGRAM_DIR/assests
        echo -e "${GREEN}Clean complete!${NC}"
        ;;
        
    "rebuild")
        echo -e "${YELLOW}Rebuilding project...${NC}"
        rm -rf $BUILD_DIR
        rm -rf $PROGRAM_DIR/$PROJECT_NAME
        rm -rf $PROGRAM_DIR/assests
        mkdir -p $BUILD_DIR
        cd $BUILD_DIR
        cmake ..
        make -j$(nproc)
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}Build successful!${NC}"
        else
            echo -e "${RED}Build failed!${NC}"
            exit 1
        fi
        ;;
        
    "run")
        if [ -f "$PROGRAM_DIR/$PROJECT_NAME" ]; then
            echo -e "${GREEN}Running $PROJECT_NAME...${NC}"
            cd $PROGRAM_DIR
            ./$PROJECT_NAME
        else
            echo -e "${RED}Executable not found! Building first...${NC}"
            ./feather.sh
            if [ -f "$PROGRAM_DIR/$PROJECT_NAME" ]; then
                cd $PROGRAM_DIR
                ./$PROJECT_NAME
            fi
        fi
        ;;
        
    *)
        echo -e "${YELLOW}Building project...${NC}"
        mkdir -p $BUILD_DIR
        cd $BUILD_DIR
        cmake ..
        make -j$(nproc)
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}Build successful!${NC}"
            echo -e "Executable location: ${YELLOW}$PROGRAM_DIR/$PROJECT_NAME${NC}"
        else
            echo -e "${RED}Build failed!${NC}"
            exit 1
        fi
        ;;
esac

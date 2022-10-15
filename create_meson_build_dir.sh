#!/bin/bash

PATH="${PATH}:${HOME}/.local/bin" CXX=g++ CXX_FOR_BUILD=g++ meson setup build

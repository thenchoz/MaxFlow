# Max-Flow Optimization Project

## Overview
The **Max-Flow Project** is an implementation of the max flow algorithm, utilizing both Python and C++ languages. This project showcases the integration of the two languages, compiled with CMake, and includes a Jupyter notebook example for demonstration purposes.

## Table of Contents
- [Max-Flow Optimization Project](#max-flow-optimization-project)
  - [Overview](#overview)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Features](#features)
  - [Installation](#installation)
    - [Prerequisites](#prerequisites)
    - [Steps](#steps)
  - [Usage](#usage)
  - [Example](#example)
    - [Usage](#usage-1)

## Introduction
The max flow algorithm is used to find the maximum flow in a directed flow network. This project implements the algorithm using C++ for the core computations and Python for the interface and additional functionalities. The Jupyter notebook included in this project provides an interactive example to demonstrate how to use this project.

## Features
- Implementation of the max flow algorithm in C++
- Python interface for easier interaction
- Jupyter notebook example to demonstrate usage
- CMake build system for compiling the C++ code

## Installation
### Prerequisites
- Python 3.x
- C++ compiler (e.g., g++)
- CMake

### Steps
1. Download this folder :
    ```sh
    cd maxflow_optimization
    ```

2. Create and activate a virtual environment:
    ```sh
    python -m venv venv
    source venv/bin/activate
    ```

3. Install Python dependencies:
    ```sh
    pip install -r python/requirements.txt
    ```

4. Compile the C++ code using CMake:
    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

## Usage
1. Ensure the virtual environment is activated:
    ```sh
    source venv/bin/activate
    ```

2. Go in the `python` folder:
    ```sh
    cd python
    ```

3. Run graph_analysis:
    ```sh
    python graph_analysis.py --help
    ```

## Example
The Jupyter notebook `MaxFlow_example.ipynb` contains an example of how to use the max flow algorithm implemented in this project. It provides a step-by-step guide to demonstrate the setup, execution, and visualization of the algorithm.

### Usage
1. Ensure the virtual environment is activated:
    ```sh
    source venv/bin/activate
    ```

2. Run the Jupyter notebook:
    ```sh
    jupyter notebook
    ```

3. Open the `MaxFlow_example.ipynb` notebook and run the cells to see the max flow algorithm in action.

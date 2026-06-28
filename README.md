# Train-Planning-system
Metro Planning System
A high-performance route planning engine for metro transit networks, featuring a Python (FastAPI) web interface powered by a C++ (Dijkstra) backend.

Architecture
Backend: FastAPI (Python) for robust API handling.
Core Engine: C++ for computationally intensive graph traversal (Dijkstra).
Binding: pybind11 for high-speed interoperability between Python and C++.
Storage: In-memory adjacency list graph for $O(1)$ routing latency.

Getting Started
Prerequisites:
Python 3.x
C++ Compiler (MSVC Build Tools on Windows)
pip install fastapi uvicorn pybind11 setuptools

Compilation
Build the C++ routing engine as a Python extension:
python setup.py build_ext --inplace
Running the API
Start the server:
python -m uvicorn app:app --reload
API Usage
Once the server is running, access the interactive documentation at:http://127.0.0.1:8000/docs
GET /stations: List all available metro stations.
GET /route: Calculate the optimal path between two stations (params: source, destination, optimization).

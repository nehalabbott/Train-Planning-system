# Train Planning System

A high-performance **Metro Route Planning System** that computes the shortest path between metro stations using **Dijkstra's Algorithm** implemented in **C++**, exposed through a **FastAPI (Python)** backend.

Demo Link:
https://drive.google.com/file/d/1WsEdF4Kaf5nVaDcXTMy3fh5NeG77scuH/view?usp=drive_link

## Features

- Shortest route calculation using Dijkstra's Algorithm
- Python-C++ interoperability using **pybind11**
- REST API built with FastAPI
- Interactive API documentation with Swagger UI
- In-memory graph representation using adjacency lists

---

## Architecture

```
                Client
                   │
             HTTP Requests
                   │
             FastAPI Backend
                (Python)
                   │
             pybind11 Binding
                   │
      C++ Route Planning Engine
          (Dijkstra Algorithm)
                   │
         Metro Graph (Adjacency List)
```

---

## 🛠️ Tech Stack

| Technology | Purpose |
|------------|---------|
| C++ | Route Planning Engine |
| Python | API Backend |
| FastAPI | REST API |
| pybind11 | Python-C++ Binding |
| Uvicorn | ASGI Server |

---

## 📂 Project Structure

```text
Train-Planning-system/
│
├── app.py                  # FastAPI application
├── setup.py                # Build configuration
├── route_engine.cpp        # Dijkstra implementation
├── route_engine.pyd/.so    # Compiled extension
├── metro_data.*            # Metro graph
├── requirements.txt
└── README.md
```

---

## 🚀 Getting Started

### Prerequisites

- Python 3.x
- C++ Compiler
  - MSVC Build Tools (Windows)
  - g++ (Linux)

Install dependencies:

```bash
pip install fastapi uvicorn pybind11 setuptools
```

---

## ⚙️ Build the C++ Extension

```bash
python setup.py build_ext --inplace
```

---

## ▶️ Run the Server

```bash
python -m uvicorn app:app --reload
```

Server will start at:

```
http://127.0.0.1:8000
```

---

## 📖 API Documentation

FastAPI automatically generates interactive documentation.

Open:

```
http://127.0.0.1:8000/docs
```

---

## 📡 API Endpoints

### Get All Stations

```http
GET /stations
```

Returns a list of all available metro stations.

---

### Find Optimal Route

```http
GET /route
```

## Core Algorithm

The routing engine uses **Dijkstra's Algorithm** to compute the shortest path in a weighted metro network.

**Time Complexity**

```
O((V + E) log V)
```

where

- **V** = Number of Stations
- **E** = Number of Connections

---




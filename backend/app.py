import subprocess
import json
import csv
from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI()

# Enable CORS so your frontend layout (React, Vue, or Vanilla HTML) can make requests
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get("/")
def home():
    return {"message": "Metro API Running"}

# Endpoint 1: Fetch all stations dynamically for dropdown menus
@app.get("/stations")
def get_stations():
    stations = []
    try:
        with open("stations.csv", "r", encoding="utf-8") as file:
            for line in file:
                clean_station = line.strip()
                if clean_station:
                    stations.append(clean_station)
        return {"stations": stations}
    except FileNotFoundError:
        raise HTTPException(status_code=404, detail="stations.csv not found.")

# Endpoint 2: Fetch calculated route from C++ engine
@app.get("/route")
def get_route(source: str, destination: str, optimization: str = "fastest"):
    try:
        # Calls: ./metro "Source Station" "Destination Station" "fastest"
        result = subprocess.run(
            ["./metro", source, destination, optimization],
            capture_output=True,
            text=True,
            check=True
        )
        
        # Parse the JSON string coming from stdout of your C++ binary
        route_data = json.loads(result.stdout)
        return route_data

    except subprocess.CalledProcessError as e:
        raise HTTPException(status_code=500, detail=f"C++ Execution Error: {e.stderr}")
    except json.JSONDecodeError:
        raise HTTPException(status_code=500, detail=f"Failed to parse backend output: {result.stdout}")
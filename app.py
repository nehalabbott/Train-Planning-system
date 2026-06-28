from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from contextlib import asynccontextmanager
import metro_routing # Your compiled C++ module

# Instantiate the C++ engine globally
engine = metro_routing.MetroSystem()

@asynccontextmanager
async def lifespan(app: FastAPI):
    # This runs exactly ONCE when the server boots
    print("Initializing C++ Engine & loading CSV to memory...")
    try:
        # Pass the path to your CSV file
        engine.load_connections("data/connections.csv")
        print("Graph loaded successfully.")
    except Exception as e:
        print(f"Failed to load data: {e}")
    
    yield # Server runs and accepts HTTP requests here
    
    print("Shutting down API...")

app = FastAPI(lifespan=lifespan)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get("/")
def home():
    return {"message": "High-Performance Metro API Running"}

@app.get("/stations")
def get_stations():
    # Calling the C++ engine to get the stations in O(1) from memory
    stations = engine.get_stations()
    if not stations:
        raise HTTPException(status_code=404, detail="No stations found.")
    return {"stations": stations}

@app.get("/route")
def get_route(source: str, destination: str, optimization: str = "fastest"):
    # Run the C++ Dijkstra algorithm
    result = engine.find_route(source, destination, optimization)
    
    if result.distance == -1:
        raise HTTPException(status_code=404, detail="Route not found between these stations.")
    
    # Convert the C++ RouteResult struct into a standard JSON dictionary
    return {
        "path": result.path,
        "distance_km": result.distance,
        "time_minutes": result.time,
        "fare_inr": result.fare,
        "engine": "C++ In-Memory Graph"
    }
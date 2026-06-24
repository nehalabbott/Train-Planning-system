from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI()

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

@app.get("/route")
def get_route(
    source: str,
    destination: str,
    optimization: str
):
    return {
        "path": [
            source,
            "Rajiv Chowk",
            destination
        ],
        "distance": 22,
        "time": 28,
        "fare": 30,
        "interchanges": 1,
        "optimization": optimization
    }
from fastapi import FastAPI
import string
import random

app = FastAPI()

# Precompute the population for random.choices to avoid repeated attribute lookups
_population = string.ascii_letters + string.digits

@app.get("/")
async def index():
    # Use SystemRandom for cryptographic randomness only if needed; here, default is fine
    mystr = ''.join(random.choices(_population, k=100))
    return {"message": mystr}

@app.get("/items/{item_id}")
async def read_item(item_id: int):
    mystr = ''.join(random.choices(_population, k=100))
    return {"item_id": item_id, "message": mystr}

# run the server with:
# uvicorn serv:app --host 0.0.0.0 --port 8000 --reload
# or
# python -m uvicorn serv:app --host 0.0.0.0 --port 8000 --reload


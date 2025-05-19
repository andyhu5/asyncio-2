from fastapi import FastAPI
import string
import random

app = FastAPI()
@app.get("/")
async def index():
    mystr = ''.join(random.choices(string.ascii_letters + string.digits, k=100))
    return {"message": mystr}

@app.get("/items/{item_id}")
async def read_item(item_id: int):
    mystr = ''.join(random.choices(string.ascii_letters + string.digits, k=100))
    return {"item_id": item_id, "message": mystr}

# run the server with:
# uvicorn serv:app --host 0.0.0.0 --port 8000 --reload
# or
# python -m uvicorn serv:app --host 0.0.0.0 --port 8000 --reload


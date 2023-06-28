from flask import Flask, request

app = Flask(__name__)

@app.route('/')
def get():
	val1 = request.args.get("이름", "user")
	val2 = request.args.get("주소", "부산")
	return val1 + " : " + val2

if __name__ == "__main__":
	app.run(host="0.0.0.0", port=8080)

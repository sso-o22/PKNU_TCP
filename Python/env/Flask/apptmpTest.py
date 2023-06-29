from flask import Flask, request, render_template

app = Flask(__name__)

@app.route('/')
def home():
	return "Hello Flask"

@app.route('/test')
def get():
	return render_template('get.html')

@app.route('/post')
def post():
	return render_template('default.html')

if __name__ == "__main__":
	app.run(host="0.0.0.0", port="9000", debug=True)

from flask import Flask

app = Flask(__name__)

@app.route('/')
def home():
	return "Flask Server Test"

@app.route('/user/<username>')  # 이름
def show_username():
	return "User : %s" % username

@app.route('/post/<int:post_id>')  # 나이
def show_post(post_id):
	return "post %d" % post_id

if __name__ == "__main__":
	app.run(host='0.0.0.0', port='9000')

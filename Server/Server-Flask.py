from flask import Flask, request, jsonify

app = Flask(__name__)
data = []

@app.route('/data', methods=['POST'])
def receive_data():
    new_data = request.json
    data.append(new_data)
    return jsonify({"status": "success"}), 200

@app.route('/data', methods=['GET'])
def get_data():
    return jsonify(data), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

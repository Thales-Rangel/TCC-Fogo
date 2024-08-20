from flask import Flask, render_template
from flask_socketio import SocketIO, emit

app = Flask(__name__)
socketio = SocketIO(app)
data = []

@app.route('/')
def index():
    return render_template('../index.html')

@socketio.on('connect')
def handle_connect():
    emit('initial_data', data)

@socketio.on('new_data')
def handle_new_data(json):
    data.append(json)
    emit('update_data', json, broadcast=True)

if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=5000)
    
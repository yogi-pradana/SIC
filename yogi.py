from flask import Flask, request, jsonify

app = Flask(__name__)

sensor_data = []

@app.route('/sensor/data', methods=['POST'])
def sensor_data_handler():
    if request.method == 'POST':
        data = request.json 
        sensor_data.append(data) 
        print("Received sensor data:", data)
        return jsonify({'message': 'Data received successfully'}), 200

@app.route('/sensor/all', methods=['GET'])
def get_all_sensor_data():
    return jsonify(sensor_data)

if __name__ == '__main__':
    app.run(debug=True, port=5000)

import socket
import json
import random
import threading

def load_questions(filename='questions.json'):
    with open(filename, 'r') as f:
        questions = json.load(f)
    return questions

def handle_client(client_socket):
    questions = load_questions()

    categories = list(set([q['category'] for q in questions]))
    
    category_message = f"Categories: {len(categories)}\n"
    category_message += "\n".join([f"{i + 1}. {category}" for i, category in enumerate(categories)])
    client_socket.send(category_message.encode())

    selected_category_index = int(client_socket.recv(1024).decode()) - 1
    selected_category = categories[selected_category_index]

    filtered_questions = [q for q in questions if q['category'] == selected_category]

    random.shuffle(filtered_questions)

    score = 0
    total_questions = len(filtered_questions)

    client_socket.send(f"Category: {selected_category}\nTotal Questions: {total_questions}".encode())

    for question in filtered_questions:
        category = question['category']
        question_text = question['question']
        options = question['options']
        correct_answer = question['correct_answer']

        options_str = '\n'.join([f"{i+1}. {opt}" for i, opt in enumerate(options)])#1. H2O
        question_message = f"{question_text}\n{options_str}"
        client_socket.send(question_message.encode())

        answer = client_socket.recv(1024).decode()

        if options[int(answer) - 1].lower() == correct_answer.lower():
            score += 1
            feedback = "Correct!"
        else:
            feedback = f"Incorrect! The correct answer was {correct_answer}."

        client_socket.send(feedback.encode())

    client_socket.send(f"Your final score: {score}/{total_questions}".encode())

    client_socket.close()

def start_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('localhost', 8000)) #"127.0.0.1"
    server.listen(5)
    print("Server started. Waiting for connections...")

    while True:
        client_socket, addr = server.accept()
        print(f"Connection from {addr} established.")
        client_thread = threading.Thread(target=handle_client, args=(client_socket,))
        client_thread.start()

if __name__ == "__main__":
    start_server()

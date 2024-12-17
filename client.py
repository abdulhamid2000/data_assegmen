import socket

def start_client():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # "127.0.0.1"
    client.connect(('localhost', 8000))

    categories_message = client.recv(1024).decode()
    print(categories_message)

    lines = categories_message.split("\n")
    total_categories = int(lines[0].split(":")[1].strip()) # categories:4

    while True:
        category_choice = input("Choose a category (enter the number): ")
        try:
            category_choice = int(category_choice)
            if category_choice < 1 or category_choice > total_categories:
                print("Invalid category number. Please choose a valid number.")
            else:
                break
        except ValueError:
            print("Please enter a valid number.")

    client.send(str(category_choice).encode())

    category_info = client.recv(1024).decode()
    print(category_info)

    while True:
        question_data = client.recv(1024).decode()

        if "Your final score" in question_data:  
            print(question_data)
            break

        print(question_data)  

        while True:
            answer = input("Your answer (Choose a number): ")
            try:
                answer = int(answer)
                if answer < 1 or answer > 4:  
                    print("Invalid answer number. Please choose a valid option.")
                else:
                    break
            except ValueError:
                print("Please enter a valid number.")

        client.send(str(answer).encode())

        feedback = client.recv(1024).decode()
        print(feedback)

if __name__ == "__main__":
    start_client()

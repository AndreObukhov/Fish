#include "pch.h"

#include "Network.h"

Network::Network() {
	//ip = "172.20.10.3";				//iPhone
	ip = "192.168.1.48";				//home
	//ip = "192.168.1.72";				//423
}

//void Network::CreateConnection() {
//    std::cout << "Enter (s) for server, enter (c) for client" << std::endl;
//    cin >> connectionType;
//
//    if (connectionType == 's') {
//        sf::TcpListener listener;
//        std::cout << "Waiting for another player..." << std::endl;
//        listener.listen(2000);
//        listener.accept(socket);
//        socket.setBlocking(false);
//        std::cout << "Player found!" << std::endl;
//    }
//    else {
//        socket.connect(ip, 2000);
//        socket.setBlocking(false);
//        std::cout << "Connection!" << std::endl;
//    }
//}

void Network::CreateConnection() {
	socket.connect(ip, 55001);
	socket.setBlocking(false);
	int players_count = 0;
	sf::Packet packet;
	while (players_count == 0) {
		socket.receive(packet);
		packet >> players_count;
	}
	switch (players_count) {
	case 0:
		std::cout << "Server doesn't work!" << std::endl;
		break;
	case 1:
		std::cout << "Waiting for another player..." << std::endl;
		while (players_count != 2) {
			socket.receive(packet);
			packet >> players_count;
		}
		std::cout << "Start the game!" << std::endl;
		break;
	case 2:
		std::cout << "Start the game!" << std::endl;
		break;
	}
}


void Network::GetAnotherFish(AnotherPlayerFish& anotherFish, std::vector<AutomaticFish>& fishes, 
								const float& client_time) {
	sf::Packet packet;
	int packet_type = 0;
	sf::Vector2f pos;
	int type;
	float angle;
	int directionType;
	float speed;
	int points = 0;
	//think about sending score

	if (socket.receive(packet) == sf::Socket::NotReady) {
		return;
	}
	packet >> packet_type;
	if (packet_type == 1) {
		if (packet >> pos.x >> pos.y >> type >> angle >> directionType >> speed >> points) {
			anotherFish.NetUpdate(pos, static_cast<FishType> (type), angle,
									static_cast<DirectionType>(directionType), speed, points);
			//testing
			//std::cout << "anotherFish recieved! " << std::endl;
		//        std::cout << anotherFish.GetPosition().x << " " << anotherFish.GetPosition().y << " "
		//                  << static_cast<int> (anotherFish.GetType()) << " "
		//                  << anotherFish.GetAngle() << " "
		//                  << static_cast<int> (anotherFish.GetDirectionType()) << " "
		//                  << anotherFish.GetSpeed() << " "
		//                  << std::endl;
		}
	}

	if (packet_type == 2) {
		if (packet >> pos.x >> pos.y >> type) {
			fishes.push_back(AutomaticFish(pos, static_cast<FishType> (type), client_time));
			std::cout << "Fish from server added sucessfully" << std::endl;
		}
	}

	if (packet_type == 3) {
		int index;
		if (packet >> index) {
			auto it = fishes.begin() + index;
			fishes.erase(it);
			std::cout << "Fish from server erased successfully" << std::endl;
		}
	}
}

void Network::SendMyFish(const ControlledFish& myFish) {
	sf::Packet packet;
	packet << 1 << myFish.GetPosition().x << myFish.GetPosition().y << static_cast<int> (myFish.GetType())
		<< myFish.GetAngle() << static_cast<int> (myFish.GetDirectionType()) << myFish.GetSpeed() << myFish.GetScore();
	socket.send(packet);
}

void Network::FishEaten(const int& index) {
	sf::Packet packet;
	packet << 3 << index;
	socket.send(packet);
}
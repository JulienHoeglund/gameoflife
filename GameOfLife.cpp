/*
Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
Any live cell with two or three live neighbours lives on to the next generation.
Any live cell with more than three live neighbours dies, as if by overpopulation.
Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/
#include<iostream>
#include<unistd.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Cell{
	private:
		int neighbors;
		bool state;
	public:
	Cell(){
		state=false;
	}
	void setNeighbors(int n){
		neighbors=n;
	}
	int getNeighbors(){
		return neighbors;
	}
	int getState(){
		int value;
		if(state==false){
			value=0;
		}
		else{
			value=1;
		}
		return value;	
	}
	void setAlive(){
		state=true;
	}
	void setDead(){
		state=false;
	}
};
class Grid{
	private:
		int X;
		int Y;
		int length;
		Cell * cells;
		bool * neighbors;
		sf::RenderWindow window;
	public:
	Grid(int x, int y)
	:X(x),Y(y){
		length=X*Y;
		cells=new Cell[length];
		neighbors=new bool[length];
		window.create(sf::VideoMode(1200,1200),"Game of life");
		window.setPosition(sf::Vector2i(350,450));
	}
	int getLength(){
		return length;
	}
	void process(){
		int n;
		for(int j=0;j<length;j++){		
			n=cells[j].getNeighbors();
			if(n<2)
				neighbors[j]=false;
			if(n==3 || (n==2 && cells[j].getState()==1))
				neighbors[j]=true;
			if(n>3)
				neighbors[j]=false;
		}
	}
	void draw(){
		sf::RectangleShape rect(sf::Vector2f(50,50));
		for(int j=0;j<length;j++){
			if(cells[j].getState()==1){
				rect.setFillColor(sf::Color(100,255,100));
			}
			else{
				rect.setFillColor(sf::Color(0,0,0));
			}
			rect.setOutlineThickness(3);
			rect.setOutlineColor(sf::Color(0,0,255));
			rect.setPosition((50*(j%X))+350,(50*((j-(j%Y)))/10)+250);
			window.draw(rect);
		}
		window.display();
	} 
	void countNeighbors(){
		int n,p;	
		for(int i=0;i<length;i++){
			n=0;
			p=i-X; 
			if((i+1)%X!=0){  
				if(p+1>0){
					if(cells[p+1].getState()==1)
						n++;
				}
			}	
			if(p>=0){
				if(cells[p].getState()==1)
					n++;
				if(i%X!=0){
					if((p-1)>=0){
						if(cells[p-1].getState()==1)
							n++;
					}
				}
			}	
			if((i+1)%X!=0){
				if((i+1)<length){
					if(cells[i+1].getState()==1){
						n++;
					}
				}
			}
			if(i%X!=0){
				if((i-1)>=0){
					if(cells[i-1].getState()==1)
						n++;
				}
			}	
			p=i+X;
			if((i+1)%X!=0){
				if(p+1<length){
					if(cells[p+1].getState()==1)
						n++;
				}
			}
			if(p<length){
				if(cells[p].getState()==1)
					n++;
			}
			if(i%X!=0){
				if((p-1)<length){
					if(cells[p-1].getState()==1)
						n++;
				}
			}
			cells[i].setNeighbors(n);
			std::cout<<"|"<<std::flush;
			std::cout<<cells[i].getState()<<std::flush;
			std::cout<<cells[i].getNeighbors()<<std::flush;
			if((i+1)%Y==0 && i>0)
				std::cout<<'\n'<<std::flush;
		}
		std::cout<<'\n'<<std::flush;
	}
	void update(){
		for(int i=0;i<length;i++){
			if(neighbors[i]==true)
				cells[i].setAlive();
			else{
				cells[i].setDead();
			}
		}
	}
	void drawConfig(int hoveredCell, bool * markedCells){
		window.clear(sf::Color::Black);
		sf::RectangleShape rect(sf::Vector2f(50,50));
		for(int j=0;j<length;j++){
			if(j==hoveredCell){
				rect.setFillColor(sf::Color(255,255,255));
			}
			else if(markedCells[j]==true){
				rect.setFillColor(sf::Color(0,0,255));
			}
			else{
				rect.setFillColor(sf::Color(0,0,0));
			}
			rect.setOutlineThickness(3);
			rect.setOutlineColor(sf::Color(0,0,255));
			rect.setPosition((50*(j%X))+350,(50*((j-(j%Y)))/10)+250);
			window.draw(rect);
		}
		window.display();
	} 
	void config(){
		bool leave=false;
		int hoveredCell=0;
		bool markedCells[length]={0};
		for(int i=0;i<length;i++){
			markedCells[i]=false;
		}
		while(!leave){
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && hoveredCell>=Y){
				hoveredCell-=Y;
				sf::sleep(sf::milliseconds(100));
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && hoveredCell<(X*Y)-Y){
				hoveredCell+=Y;
				sf::sleep(sf::milliseconds(100));
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && hoveredCell>0){
				hoveredCell-=1;
				sf::sleep(sf::milliseconds(100));
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && (hoveredCell%Y)<Y){
				hoveredCell+=1;
				sf::sleep(sf::milliseconds(100));
			}
			drawConfig(hoveredCell,markedCells);
	
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				markedCells[hoveredCell]=true;
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
				markedCells[hoveredCell]=false;
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::N)){
				leave=true;
			}
		}
		for(int i=0;i<length;i++){
			if(markedCells[i]==true)
				cells[i].setAlive();
		}				
	}
	void run(){		
		bool running=false;		
		while(window.isOpen()){
			sf::Event event;
			if(!running){
				config();
				running=true;
			}
			sf::sleep(sf::milliseconds(1000));
			while(window.pollEvent(event)){
				if(event.type==sf::Event::Closed)
					window.close();
			}
			window.clear(sf::Color::Black);
			if(running){
				countNeighbors();
				process();
				update();
			}
			draw();
		}
	}

};
int main(){
	int X = 10;
	int Y = 10;
	Grid grid(X,Y);
	grid.run();
		
	return 0;
}
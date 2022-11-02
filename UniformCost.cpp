#include <bits/stdc++.h>
using namespace std;

int expand = 0; //keeps track of total expansions
bool exists = false; //tells whether a repeat exists
vector<vector<vector<int>>> repeat = {{{0}}}; //holds repeated configurations
int queue_sz = 0; //holds recorded max queue size

//TREE NODE STRUCTURE
struct node{
    vector<vector<int>> config = {{0}};
    int blankr; //the blank row number
    int blankc; //blank column number
    int dp; //depth of the current node in the tree
};

node* Make_Node(vector<vector<int>>, int dep); //function declaration


//ARCHITECTURE OF THE PROBLEM
struct problem{ 
      
    vector<vector<int>> initial_state = {{0}}; 

    //Operators
    vector<vector<int>> move_up(node* n){//return a config with blank swapped with value above it
      int temp1;
      
      vector<vector<int>>temp0 = n->config;
      
      temp1 = temp0[n->blankr - 1][n->blankc];
      temp0[n->blankr - 1][n->blankc] = 0;
      temp0[n->blankr][n->blankc] = temp1;

      return temp0;
    }
    vector<vector<int>> move_down(node* n){//return a config with blank swapped with value below it
      int temp1;
      
      vector<vector<int>>temp0 = n->config;
      
      temp1 = temp0[n->blankr + 1][n->blankc];
      temp0[n->blankr + 1][n->blankc] = 0;
      temp0[n->blankr][n->blankc] = temp1;

      return temp0;
    }
    vector<vector<int>> move_left(node* n){//return a config with blank swapped with value to the left
      int temp1;
      
      vector<vector<int>>temp0 = n->config;
      
      temp1 = temp0[n->blankr][n->blankc - 1];
      temp0[n->blankr][n->blankc - 1] = 0;
      temp0[n->blankr][n->blankc] = temp1;

      return temp0;
    }
    vector<vector<int>> move_right(node* n){//return a config with blank swapped with value to the right
      int temp1;
      
      vector<vector<int>>temp0 = n->config;
      
      temp1 = temp0[n->blankr][n->blankc + 1];
      temp0[n->blankr][n->blankc + 1] = 0;
      temp0[n->blankr][n->blankc] = temp1;

      return temp0;
    }

    vector<vector<int>> goal_state = { 
    {1,2,3},
    {4,5,6},
    {7,8,0}
  };

};

//EXPAND
queue<node*> Expand(node* n,problem p){
  queue<node*> q;
  ++expand; //increment expansion
  //creates possible children based on blank position
  if(n->blankr == 0){ //top row
    if(n->blankc == 0){
       q.push(Make_Node(p.move_down(n),n->dp));
       q.push(Make_Node(p.move_right(n),n->dp));
       return q;
    }
    else if(n->blankc == 1 ){
      q.push(Make_Node(p.move_down(n),n->dp));
      q.push(Make_Node(p.move_left(n),n->dp));
      q.push(Make_Node(p.move_right(n),n->dp));
      return q;
    }
    else if(n->blankc == 2){
      q.push(Make_Node(p.move_down(n),n->dp));
      q.push(Make_Node(p.move_left(n),n->dp));
      return q;
    }
  }
  else if(n->blankr == 1){ //middle row
    if(n->blankc == 0){
       q.push(Make_Node(p.move_up(n),n->dp));
       q.push(Make_Node(p.move_down(n),n->dp));
       q.push(Make_Node(p.move_right(n),n->dp));
       return q;
    }
    else if(n->blankc == 1 ){
      q.push(Make_Node(p.move_up(n),n->dp));
      q.push(Make_Node(p.move_down(n),n->dp));
      q.push(Make_Node(p.move_left(n),n->dp));
      q.push(Make_Node(p.move_right(n),n->dp));
      return q;
    }
    else if(n->blankc == 2){
      q.push(Make_Node(p.move_up(n),n->dp));
      q.push(Make_Node(p.move_down(n),n->dp));
      q.push(Make_Node(p.move_left(n),n->dp));
      return q;
    }
  }
  else if(n->blankr == 2){ //bottom row
    if(n->blankc == 0){
       q.push(Make_Node(p.move_up(n),n->dp));
       q.push(Make_Node(p.move_right(n),n->dp));
       return q;
    }
    else if(n->blankc == 1 ){
      q.push(Make_Node(p.move_up(n),n->dp));
      q.push(Make_Node(p.move_left(n),n->dp));
      q.push(Make_Node(p.move_right(n),n->dp));
      return q;
    }
    else if(n->blankc == 2){
      q.push(Make_Node(p.move_up(n),n->dp));
      q.push(Make_Node(p.move_left(n),n->dp));
      return q;
    }
  }

  return q;
}

//UNIFORM COST SEARCH IMPLEMENTATION
queue<node*> Queueing_Function(queue<node*> q, queue<node*> add_on){

  while(!add_on.empty()){ //adds the expanded nodes to the back of the main frontier queue
    for(int i = repeat.size() - 1; i > -1; --i){ //check for repeated states
      if(repeat.at(i) == add_on.front()->config){
        exists = true;
        break;
      }
    }
    if(!exists){ //if the current node in question is not a repeat
      q.push(add_on.front());
      repeat.push_back(add_on.front()->config);
    }
    add_on.pop();
    exists = false;
  }
  
  return q;
};  

//MAKE NODE FUNCTION
node* Make_Node(vector<vector<int>> start, int dep){

    node* node1 = new node;

    node1->config = start; //set new node's config
    node1->dp = dep + 1; //adds one to the parent depth to get the child depth
    
    for(int i = 0; i < 3; ++i){//locates the blank positon in newly created node
      for(int j = 0; j < 3; ++j){
        if(node1->config[i][j] == 0){ 
          node1->blankr = i;
          node1->blankc = j;
          return node1;
        }
      }
    }

    return node1;
};

//MAKE QUEUE FUNCTION
queue<node*> Make_Queue(node* start){//make a queue with given root node
    queue<node*> q;
    q.push(start);
    return q;
};

//uniform cost search/BFS case
void generic_search(problem p1){
  
  queue<node*> frontier  = Make_Queue(Make_Node(p1.initial_state,-1)); //create root node and add it to the stack
  repeat.push_back(p1.initial_state);
  
  while(1){ //loop of actual search
    if(frontier.empty()){ //failure condition
      printf("FAILURE, NO SOLUTION EXISTS\n");
      return;
    }
    
    node* n = Make_Node(frontier.front()->config,frontier.front()->dp - 1); //copy potential expanded node and decrease its depth bc makenode adds one by default
    if(frontier.size() > queue_sz){ //check the size of the queue to update the maximum size
      queue_sz = frontier.size();
    }
    frontier.pop(); //removes node at front of queue

    if(p1.goal_state == n->config){//goal state check
      printf("SOLUTION HAS BEEN FOUND with config: \n{%d,%d,%d},\n{%d,%d,%d},\n{%d,%d,%d}\n", n->config[0][0], n->config[0][1], n->config[0][2], n->config[1][0], n->config[1][1], n->config[1][2], n->config[2][0], n->config[2][1],n->config[2][2]);
      printf("Solution at depth: %d\n", n->dp);
      printf("Number of nodes expanded: %d\n", expand);
      printf("Maximum Queue size: %d\n", queue_sz);
      return;
    }

      printf("Expanding node with the following configuration: \n{%d,%d,%d},\n{%d,%d,%d},\n{%d,%d,%d}\n", n->config[0][0], n->config[0][1], n->config[0][2], n->config[1][0], n->config[1][1], n->config[1][2], n->config[2][0], n->config[2][1],n->config[2][2]);
      frontier = Queueing_Function(frontier, Expand(n,p1));
    //delete[] n; //done with temp variable n to hold frontier front node to be expanded
  }

  return;
}

int main(){
  
  problem problem1; //uncomment a problem state to test or create an additonal starting state
  
  char input = 'a';
  char input1, input2, input3, input4, input5, input6, input7, input8, input9;
  int num = 0;
  vector<vector<int>> custom = {
        {1,2,3},
        {4,5,6},
        {7,8,0}
      };

  printf("Welcome to my 8-puzzle solver Uniform Cost Search Edition, Type '1' to use a default puzzle, or type '2' to create your own:\n");
  scanf("%c", &input);
  
  num = input - '0';


  if(num == 1){
    printf("You have chosen a default puzzle with solution at depth 8: ");
      problem1.initial_state =  {
        {1,3,6},
        {5,0,2},
        {4,7,8}
      };
  }

  else if(num == 2){
    printf("You have chosen a custom puzzle.\n");
    
    printf("Please enter a valid number for row 1 column 1:\n");
    cin >> input1;
    custom.at(0).at(0) = input1 - '0';

    printf("Please enter a valid number for row 1 column 2:\n");
    cin >> input2;
    custom.at(0).at(1) = input2 - '0';

    printf("Please enter a valid number for row 1 column 3:\n");
    cin >> input3;
    custom.at(0).at(2) = input3 - '0';
    
    printf("Please enter a valid number for row 2 column 1:\n");
    cin >> input4;
    custom.at(1).at(0) = input4 - '0';
    
    printf("Please enter a valid number for row 2 column 2:\n");
    cin >> input5;
    custom.at(1).at(1) = input5 - '0';

    printf("Please enter a valid number for row 2 column 3:\n");
    cin >> input6;
    custom.at(1).at(2) = input6 - '0';

    printf("Please enter a valid number for row 3 column 1:\n");
    cin >> input7;
    custom.at(2).at(0) = input7 - '0';

    printf("Please enter a valid number for row 3 column 2:\n");
    cin >> input8;
    custom.at(2).at(1) = input8 - '0';

    printf("Please enter a valid number for row 3 column 3:\n");
    cin >> input9;
    custom.at(2).at(2) = input9 - '0';

  }

  problem1.initial_state =  custom;
  

  // problem1.initial_state =  {
  //   {1,2,3},
  //   {4,5,6},
  //   {7,8,0}
  // };

//  problem1.initial_state =  {
//     {1,2,3},
//     {4,5,6},
//     {0,7,8}
//   };

// problem1.initial_state =  {
//     {1,2,3},
//     {5,0,6},
//     {4,7,8}
//   };

// problem1.initial_state =  {
//     {1,3,6},
//     {5,0,2},
//     {4,7,8}
//   };

// problem1.initial_state =  {
//     {1,3,6},
//     {5,0,7},
//     {4,8,2}
//   };

// problem1.initial_state =  {
//     {1,6,7},
//     {5,0,3},
//     {4,8,2}
//   };

// problem1.initial_state =  {
//     {7,1,2},
//     {4,8,5},
//     {6,3,0}
//   };

  generic_search(problem1); //call search alorithm



  return 0;
}
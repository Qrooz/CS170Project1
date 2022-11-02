#include <bits/stdc++.h>
using namespace std;

int expand = 0;
bool exists = false;
vector<vector<vector<int>>> repeat = {{{0}}};
int queue_sz = 0;

//TREE NODE STRUCTURE
struct node{
    vector<vector<int>> config = {{0}};
    int blankr; //the blank row number
    int blankc; //blank column number
    int dp; //depth of the current node in the tree
    int h = 0; //misplaced tile count 
    int f = 0; //heuristic
};

struct less_than{//struct to be used in order to create 
  inline bool operator() (node* item1, node* item2){
      return (item1->f < item2->f);
  }
};

node* Make_Node(vector<vector<int>>, int dep);

queue<node*> sort_q(queue<node*> q){//helper funtion to sort the queue into a prioirity queue based on heuristic value

  vector<node*> temp;
  queue<node*> n;

  while(!q.empty()){
    temp.push_back(q.front());
    q.pop();
  }

  sort(temp.begin(),temp.end(), less_than());

  for(int i = 0; i < temp.size(); ++i){
      n.push(temp.at(i));
  }

  return n;
}


//ARCHITECTURE OF THE PROBLEM FIXME FINISH OPERATOR IMPLENTATION
struct problem{ 
      
    vector<vector<int>> initial_state = {{0}}; 

    //Operators
    vector<vector<int>> move_up(node* n){
      int temp1;
      
      vector<vector<int>>temp0 = n->config;
      
      temp1 = temp0[n->blankr - 1][n->blankc];
      temp0[n->blankr - 1][n->blankc] = 0;
      temp0[n->blankr][n->blankc] = temp1;

      return temp0;
    }
    vector<vector<int>> move_down(node* n){
      int temp1;
      
      vector<vector<int>>temp0 = n->config;
      
      temp1 = temp0[n->blankr + 1][n->blankc];
      temp0[n->blankr + 1][n->blankc] = 0;
      temp0[n->blankr][n->blankc] = temp1;

      return temp0;
    }
    vector<vector<int>> move_left(node* n){
      int temp1;
      
      vector<vector<int>>temp0 = n->config;
      
      temp1 = temp0[n->blankr][n->blankc - 1];
      temp0[n->blankr][n->blankc - 1] = 0;
      temp0[n->blankr][n->blankc] = temp1;

      return temp0;
    }
    vector<vector<int>> move_right(node* n){
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
  ++expand;
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
    for(int i = repeat.size() - 1; i > -1; --i){ 
      if(repeat.at(i) == add_on.front()->config){
        exists = true;
        break;
      }
    }
    if(!exists){
      q.push(add_on.front());
      repeat.push_back(add_on.front()->config);
    }
    add_on.pop();
    exists = false;
  }

  //q = sort_q(q);
  
  return q;
};  

//MAKE NODE FUNCTION
node* Make_Node(vector<vector<int>> start, int dep){

    node* node1 = new node;

    vector<vector<int>> goal = {
      {1,2,3},
      {4,5,6},
      {7,8,0}
    };

    node1->config = start; //set new node's config to 
    node1->dp = dep + 1; //adds one to the parent depth to get the child depth
    node1->h = 0;

    for(int k = 0; k < 3; ++k){ //for loops for calculated the misplaced tile heuristic
      for(int l = 0; l < 3; ++l){
        if(node1->config[k][l] != goal[k][l]){
          ++node1->h;
        }
      }
    }
    
    node1->f = node1->h + node1->dp;
    
    for(int i = 0; i < 3; ++i){ //for loops for tracking the blank's position
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
queue<node*> Make_Queue(node* start){
    
    queue<node*> q;
    q.push(start);
    return q;
};

//uniform cost search misplaced tile
void generic_search(problem p1){
  
  queue<node*> frontier  = Make_Queue(Make_Node(p1.initial_state,-1)); //create root node and add it to the stack
  repeat.push_back(p1.initial_state);
  
  while(1){ //loop of actual search
    if(frontier.empty()){
      printf("FAILURE, NO SOLUTION EXISTS\n");
      return;
    }
    
    node* n = Make_Node(frontier.front()->config,frontier.front()->dp - 1); //copy potential expanded node and decrement its depth bc makenode adds one
    if(frontier.size() > queue_sz){ //check the size of the queue to update the maximum size
      queue_sz = frontier.size();
    }
    frontier.pop(); //removes node at front of queue

    if(p1.goal_state == n->config){
      printf("SOLUTION HAS BEEN FOUND with config: \n{%d,%d,%d},\n{%d,%d,%d},\n{%d,%d,%d}\n", n->config[0][0], n->config[0][1], n->config[0][2], n->config[1][0], n->config[1][1], n->config[1][2], n->config[2][0], n->config[2][1],n->config[2][2]);
      printf("Solution at depth: %d\n", n->dp);
      printf("Number of nodes expanded: %d\n", expand);
      printf("Maximum Queue size: %d\n", queue_sz);
      return;
    }
      printf("Expanding node at depth %d with h(n) = %d configuration: \n{%d,%d,%d},\n{%d,%d,%d},\n{%d,%d,%d}\n", n->dp, n->h, n->config[0][0], n->config[0][1], n->config[0][2], n->config[1][0], n->config[1][1], n->config[1][2], n->config[2][0], n->config[2][1],n->config[2][2]);
      frontier = sort_q(Queueing_Function(frontier, Expand(n,p1)));
    //delete[] n; //done with temp variable n to hold frontier front node to be expanded
  }

  return;
}

int main(){
  
  problem problem1;
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

problem1.initial_state =  {
    {0,7,2},
    {4,6,1},
    {3,5,8}
  };



  generic_search(problem1);


  return 0;
}
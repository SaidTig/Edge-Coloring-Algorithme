simple Node
{
	parameters:
	    int nbr_colors; 
    gates:
	        inout gate[] @loose;

	    	
}
network Edge_Coloring
{
    parameters:
        int count ;
        double connectedness; // 0.0<x<1.0
        int number_colors;
        
	types:
		channel Channel extends ned.DelayChannel {
			delay = 100ms;  
	        }
    submodules:
        node[count]: Node;
        
    connections allowunconnected:
        
        for i=0..count-1, for j=i..count-1, if i!=j && uniform(0,1)<connectedness {
			node[i].gate++ <--> {  delay = 100ms; } <--> node[j].gate++;
        }
}
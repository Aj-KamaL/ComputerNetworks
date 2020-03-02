import java.util.ArrayList;
import java.util.HashSet;
import java.util.Scanner;
public class link_state
{		
	public void showroute(int[] parents,int presentroter) 
    {
        if (presentroter == -1) 
        {return;} 
        showroute(parents, parents[presentroter]); 
        System.out.print(presentroter + " "); 
    }
    public  void shortest_path( int origin,int[][] network) 
    { 
        int v_count = network[0].length;  
        int[] iscovered = new int[v_count]; 
        int[] least_dist = new int[v_count];       
        for (int i = 0; i< v_count;i++) 
        { 
            least_dist[i] = Integer.MAX_VALUE; 
        }        
        int[] recent = new int[v_count]; 
        recent[origin] = -1; 
        least_dist[origin] = 0; 
        for (int i = 1; i < v_count; i++) 
        { 
            int neighbour = -1; 
            int nearestr_dist = Integer.MAX_VALUE; 
            for (int j = 0;j < v_count;j++) 
            { 
                if (iscovered[j]==0&&least_dist[j] <nearestr_dist)  
                { 
                	neighbour = j; 
                	nearestr_dist = least_dist[j]; 
                } 
            } 
            iscovered[neighbour] = 1; 
            for (int j = 0;j < v_count;j++)  
            { 
                int cost = network[neighbour][j];                  
                if (cost>0&&(least_dist[j]>(nearestr_dist + cost)))  
                { 
                    recent[j] = neighbour; 
                    least_dist[j] = nearestr_dist+cost ; 
                    extract(recent, origin, least_dist);
                } 
            } 
        }  
        extract(recent,origin, least_dist); 
    } 
    public void extract(int[] parents,int origin,int[] distances) 
    { 
    	System.out.println();         
        System.out.print("Routers\t\tRoute");  
        int v_count = distances.length;
        for (int i = 0;i < v_count;i++)  
        { 
            if (i != origin)  
            { 
                System.out.print("\n" + origin + "    "); 
                System.out.print(i + " \t\t "); 
               // System.out.print(distances[vertexIndex] + "\t\t"); 
                showroute(parents,i); 
            } 
        } 
    } 
    
	public static void main(String [] args) 
	{
		
		Scanner sc =new Scanner (System.in);
		String line =sc.nextLine();
		String[] gp=line.split(";");
		ArrayList<Integer> src=new ArrayList<>();
		ArrayList<Integer> dest=new ArrayList<>();
		ArrayList<Integer> cost=new ArrayList<>();
		for(int i=0;i<gp.length;i++) 
		{
			String[] temp=gp[i].split(",");
			cost.add(Integer.parseInt(temp[1].replaceAll("\\s","")));
			String[] temp2=temp[0].split(":");
			src.add(Integer.parseInt(temp2[0].replaceAll("\\s","")));
			dest.add(Integer.parseInt(temp2[1].replaceAll("\\s","")));		
			
			
		}
		int vertices=0;
		HashSet noDupSet = new HashSet();
		for(int i=0;i<src.size();i++) 
		{			
			noDupSet.add(src.get(i));
			noDupSet.add(dest.get(i));
		}
		vertices=noDupSet.size();
		int edges=cost.size();			
		int[][] network = new int[vertices][vertices];
		int[][] parent = new int[vertices][vertices];
		int[][] route = new int[vertices][vertices];
		for(int i = 0; i < vertices; i++)
		{ 
		   for(int j = 0; j < vertices; j++)
		   {
			    if(i == j)
			    {
			     network[i][j] = 0;
			    }
			    else
			    {
			     network[i][j] =1000000;
			    }
			   }
		}
		for(int i=0;i<src.size();i++) {
			network[src.get(i)-1][dest.get(i)-1]=cost.get(i);
			network[dest.get(i)-1][src.get(i)-1]=cost.get(i);			
		}
		link_state sample=new link_state();
		sample.shortest_path(0, network);		
	}
}

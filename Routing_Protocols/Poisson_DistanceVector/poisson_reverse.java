import java.util.ArrayList;
import java.util.HashSet;
import java.util.Scanner;
public class poisson_reverse 
{	
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
		for(int i=0;i<src.size();i++) {
			
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
		for(int i = 0; i < vertices; i++)
		  {
		   for(int j = 0; j < vertices; j++)
		   {
		    if(i == j)
		    {
		     route[i][j] = 0;
		     parent[i][j] = i;
		    }
		    else
		    {
		     route[i][j] = 1000000;
		     parent[i][j] = 100;
		    }
		   }
		  }	
		poisson_reverse sample=new poisson_reverse();
		sample.inform(network,vertices,route,parent);
		System.out.println("Routing Tables : ");
		System.out.println("*******************************************");
		  for(int i=0; i<vertices;i++)
		  {
		   for(int j=0;j<vertices;j++)
		   {
			   System.out.print(route[i][j] +" "+"    ");
		   }
		   System.out.println();
		  }
		
		while(true) {
			System.out.print("Enter the Source : ");
			int s = Integer.parseInt(sc.nextLine())-1;
			System.out.print("Enter the Destination : ");
			int d = Integer.parseInt(sc.nextLine())-1;			
			System.out.print("Enter the new cost: ");
			int c = Integer.parseInt(sc.nextLine());
			network[s][d] = c;
			network[d][s] = c;
			//route[s][d]=c;
			//route[d][s]=c;
			
			for(int i = 0; i < vertices; i++)
			  {
			   for(int j = 0; j < vertices; j++)
			   {
			    if(i == j)
			    {
			     route[i][j] = 0;
			     parent[i][j] = i;
			    }
			    else
			    {
			     route[i][j] = 1000000;
			     parent[i][j] = 100;
			    }
			   }
			  }
			
			sample.inform(network,vertices,route,parent);
			System.out.println("Updated Routing Tables");
			System.out.println("**************************************");			
			
			  for(int i=0; i<vertices;i++)
			  {
			   for(int j=0;j<vertices;j++)
			   {
				   System.out.print(route[i][j] +" "+"    ");
			   }
			   System.out.println();
			  }
		}	
	}

	 public void neighbourexchange(int source,int [][] network,int v,int [][] route,int [][] parent)
	 {
	  for(int i = 0; i < v; i++)
	  {
	   if(network[source][i] != 1000000)
	   {
	    int dist = network[source][i];
	    for(int j = 0; j < v; j++)
	    {
	     int inter_dist = route[i][j];
	     if(parent[i][j] == source)
	      inter_dist = 1000000;
	     if(dist + inter_dist < route[source][j])
	     {
	    	 for(int l=0; l<v;l++)
			  {
			   for(int m=0;m<v;m++)
			   {
				   System.out.print(route[l][m] +" "+"    ");
			   }
			   System.out.println();
			  }
	      route[source][j] = dist + inter_dist;
	      parent[source][j] = i;
	     }
	    }
	   }
	  }

	  
	 }
	 
	 public void inform(int [][] network,int v,int [][] route,int [][] parent)
	 {
	  int index = 0;
	  int temp=4*v;
	  for(int i = 0; i < temp; i++)
	  {
	   neighbourexchange(index,network,v,route,parent);
	   index=index+1;
	   if(index == v)
	    index = 0;
	  }
	 }
	 
	
	 
	 

}

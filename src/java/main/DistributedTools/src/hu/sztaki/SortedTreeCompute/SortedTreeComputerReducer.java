/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hu.sztaki.SortedTreeCompute;

import java.io.IOException;
import java.util.Iterator;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class SortedTreeComputerReducer extends
        Reducer<TreeKey, Edge, Text, Text>  {
  @Override
  public void reduce(TreeKey key, Iterable<Edge> values, Context context)
          throws InterruptedException, IOException {
    Iterator<Edge> edges = values.iterator();
    
    Edge edge;
    Edge prevEdge = null;
    StringBuilder builder = new StringBuilder();
    Text outputKey = new Text(key.getFpindex() + " " +
    key.getRootNode() + " " + key.getTreeSize());
    //System.out.println("key " + key.getFpindex() + " " + key.getRootNode());
    
    while (edges.hasNext()) {
      edge = new Edge(edges.next());
      if (prevEdge != null &&
              prevEdge.getPathIndex() == edge.getPathIndex() &&
              prevEdge.getStartNode() == edge.getStartNode() &&
              prevEdge.getEndNode() == edge.getEndNode()) {
        continue;
      }
      prevEdge = edge;
      
      //System.out.println(edge.getPathIndex() + " " + edge.getStartNode() +
      //        " " + edge.getEndNode());
      builder.append("|" + edge.getPathIndex() + " " + edge.getStartNode() +
              " " + edge.getEndNode());
    }
   
    context.write(outputKey, new Text(builder.toString()));
  }
}

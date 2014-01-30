/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hu.sztaki.TreeCompute;

import java.io.IOException;
import java.util.TreeSet;
import java.util.Iterator;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;


public class TreeComputerReducer extends
        Reducer<TreeKey, FPTreeNode, Text, Text> {
  
  @Override
  public void reduce(TreeKey key, Iterable<FPTreeNode> values, Context context) 
      throws IOException, InterruptedException {
    Iterator<FPTreeNode> nodes = values.iterator();
    
    FPTreeNode node;
    StringBuilder buffer = new StringBuilder();
    TreeSet<FPTreeNode> edges = new TreeSet<FPTreeNode>();
    
    while (nodes.hasNext()) {
      node = nodes.next();
      //System.out.println("fromred|" + node.getPathIndex() + " " + node.getStartNode() +
      //        " " + node.getEndNode());
      edges.add(new FPTreeNode(node.getPathIndex(), node.getStartNode(),
              node.getEndNode()));
    }
    
    Iterator<FPTreeNode> sortedEdges = edges.iterator();
    while (sortedEdges.hasNext()) {
      node = sortedEdges.next();
      //System.out.println("sortedset|" + node.getPathIndex() + " " + node.getStartNode() +
      //        " " + node.getEndNode());
      
      buffer.append("|" + node.getPathIndex() + " " + node.getStartNode() +
              " " + node.getEndNode());
    }
    
    Text outputKey = new Text(new String(key.getFpindex()
            + " " + key.getRootNode()));
    context.write(outputKey, new Text(buffer.toString()));
  }

}

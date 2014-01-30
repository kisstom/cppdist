/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hu.sztaki.TreeCompute;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Writable;


public class FPTreeNode implements Writable, Comparable<FPTreeNode>  {
  private IntWritable pathIndex_;
  private LongWritable startNode_;
  private LongWritable endNode_;
  
  public FPTreeNode() {
    pathIndex_ = new IntWritable();
    startNode_ = new LongWritable();
    endNode_ = new LongWritable();    
  }
  
  public FPTreeNode(int pathI, long startN, long endN) {
    pathIndex_ = new IntWritable(pathI);
    startNode_ = new LongWritable(startN);
    endNode_ = new LongWritable(endN);    
  }
  
  public int getPathIndex() {
    return pathIndex_.get();
  }
  
  public long getStartNode() {
    return startNode_.get();
  }
  
  public long getEndNode() {
    return endNode_.get();
  }
  
  @Override
  public void write(DataOutput out) throws IOException {
    pathIndex_.write(out);
    startNode_.write(out);
    endNode_.write(out);
  }

  @Override
  public void readFields(DataInput in) throws IOException {
    pathIndex_.readFields(in);
    startNode_.readFields(in);
    endNode_.readFields(in);
  }

  @Override
  public int compareTo(FPTreeNode o) {
    final FPTreeNode other = (FPTreeNode) o;
    if (this.getPathIndex() > other.getPathIndex()) {
      return 1;
    } else if (this.getPathIndex() < other.getPathIndex()) {
      return -1;
    }
    
    if (this.getStartNode() > other.getStartNode()) {
      return 1;
    } else if (this.getStartNode() < other.getStartNode()) {
      return -1;
    }
    
    return 0;
  }

}

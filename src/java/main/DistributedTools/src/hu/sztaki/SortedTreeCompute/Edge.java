/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hu.sztaki.SortedTreeCompute;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Writable;


public class Edge implements Writable {
  private IntWritable pathIndex_;
  private LongWritable startNode_;
  private LongWritable endNode_;
  
  public Edge () {
    pathIndex_ = new IntWritable();
    startNode_ = new LongWritable();
    endNode_ = new LongWritable();
  }
  
  public Edge (Edge edge) {
    pathIndex_ = new IntWritable(edge.getPathIndex());
    startNode_ = new LongWritable(edge.getStartNode());
    endNode_ = new LongWritable(edge.getEndNode());
  }
  
  public Edge (int pi, long st, long en) {
    pathIndex_ = new IntWritable(pi);
    startNode_ = new LongWritable(st);
    endNode_ = new LongWritable(en);
  }
  
  public long getStartNode() {
    return startNode_.get();
  }
  
  public long getEndNode() {
    return endNode_.get();
  }
  
  public int getPathIndex() {
    return pathIndex_.get();
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

}

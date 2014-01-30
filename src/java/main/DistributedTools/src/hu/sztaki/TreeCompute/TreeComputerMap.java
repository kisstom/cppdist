/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hu.sztaki.TreeCompute;
import java.io.IOException;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class TreeComputerMap extends
        Mapper<LongWritable, Text, TreeKey, FPTreeNode> {
  
  @Override
  public void map(LongWritable key, Text value, Context context) 
      throws IOException, InterruptedException {
    String line = value.toString();
    String[] spl = line.split(" ");
    
    if (spl.length < 3) {
      return;
    }
    
    int fpIndex = Integer.parseInt(spl[0]);
    long root = Long.parseLong(spl[1]);
    TreeKey treeKey = new TreeKey(fpIndex, root);
    
    for (int i = 2; i < spl.length - 1; ++i) {
      context.write(treeKey, new FPTreeNode(i - 2, Long.parseLong(spl[i]),
              Long.parseLong(spl[i + 1])));
    }
  }
}

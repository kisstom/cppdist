/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package hu.sztaki.SortedTreeCompute;
import java.io.IOException;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class SortedTreeComputerMap extends
        Mapper<LongWritable, Text, TreeKey, Edge> {
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
        
    for (int i = 2; i < spl.length - 1; ++i) {
      context.write(new TreeKey(fpIndex, root, i - 2, Long.parseLong(spl[i]),
              Long.parseLong(spl[i + 1])), new Edge(i - 2, Long.parseLong(spl[i]),
              Long.parseLong(spl[i + 1])));
    }
  }
}

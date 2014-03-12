node_topic_file=/mnt/store/home/kisstom/data/portugal/node_topic
hash_node=/mnt/store/home/kisstom/data/portugal/reindex_hash.txt
url_hash=/mnt/store/home/kisstom/data/portugal/url_hash.txt
url_topic=/mnt/store/home/kisstom/data/portugal/url_topic

for node in 350012 427641 2342719 21885289 43082933 45051641 94081591 162545601 179208656 182840801 245300138
do
 echo "node ", $node

 topic=`egrep ^$node" " $node_topic_file | awk '{print $2}'`
 echo "topic",$topic

 hash=`egrep " "$node"$" $hash_node |awk '{print $1}'`
 echo "hash", $hash

 url=`egrep " "$hash"$" $url_hash|awk '{print $1}'`
 echo "url", $url

 other_topic=`egrep "^"$url" " $url_topic|awk '{print $2}'`
 echo "t1, t2", $topic, $other_topic
done

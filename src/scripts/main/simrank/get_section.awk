BEGIN{
 p = "null"
 meet[0]
 delete meet
 ind = 0
} 
{
  if (p != $1","$field_num) {
    for (i=1; i < ind; ++i) {
      for (j=1; j < i; ++j) {
        if (meet[i] < meet[j]) {
	  pair = meet[i]","meet[j]
	} else {
	  pair = meet[j]","meet[i]
	}
        
        section_list[pair]   
      }
    }

    delete meet
    ind = 1
    p = $1","$field_num
  }

  if (NF < field_num) next
  meet[ind++] = $2

}
END{
    for (i=1; i < ind; ++i) {
      for (j=1; j < i; ++j) {
        if (meet[i] < meet[j]) {
	  pair = meet[i]","meet[j]
	} else {
	  pair = meet[j]","meet[i]
	}
        section_list[pair]   
      }
    }


  for (p in section_list) {
    split(p, spl, ",")
    print spl[1], spl[2]
  }
}

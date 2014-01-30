function fill_section() {
  countX = 0
  for (x in meet) {
    ++countX
    countY = 0
    for (y in meet) {
      ++countY
      if (countY == countX) break
      if (x < y) {
        pair = x","y
      } else {
        pair = y","x
      }
      section_list[pair]   
    }
  }

  delete meet
  p = $1","$field_num
}



BEGIN{
 p = "null"
 meet[0]
 delete meet
} 
{
  if (p != $1","$field_num) {
    fill_section()
  }

  if (NF < field_num) next
  meet[$2]

}
END{
  fill_section()

  for (p in section_list) {
    split(p, spl, ",")
    print spl[1], spl[2]
  }
}

BEGIN{
  srand(seed)
  LONGMAX = 4294967296
  SHORTMAX = 32768
  
  for (i = 1; i <= int (numline); ++i) {
    print int (rand() * LONGMAX), int (rand() * SHORTMAX), int (rand() * SHORTMAX)
  }
} 
{}

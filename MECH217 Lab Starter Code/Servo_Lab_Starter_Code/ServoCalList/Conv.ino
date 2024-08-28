float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{   // a floating point version of the map() function if you need one
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#!/usr/bin/env gawk

LC_ALL=C

function append(str,sp)
{
    split(str,arr,/[^0-9]+/)
    for (t in arr)
    {
      if ( match(arr[t],/[0-9]+/) )
        number = number sp arr[t]
    }
}

/^SET[ ]+\([ ]*XHTMLDBG_VERSION_MAJOR[ ]+[0-9][ ]*\)$/ {
  append($0,"")
}

/^SET[ ]+\([ ]*XHTMLDBG_VERSION_MINOR[ ]+[0-9][ ]*\)$/ {
  append($0,".")
}

/^SET[ ]+\([ ]*XHTMLDBG_VERSION_RELEASE[ ]+[0-9][ ]*\)$/ {
  append($0,".")
}

END {
  print number
}

# eof

#/bin/bash
find . -regex '.*\.\(cpp\|h\|hpp\)' -exec clang-format -style=file -i {} \;
```
struct StackMachine {
    std::stack;
    std::unordered_map<std::string, size_t>;
    top_addr 0x010
}
```

```
if(1 == 2) {
    b = 1;
}
else {
    b = 2;
}
```
```as
entry:
    PUSH 1
    PUSH 2
    EQ //0
    JE bb0 bb1
bb0:
    PUSH 0x100
    PUSH 1
    STORE
    JMP end
bb1:
    PUSH 0x100
    PUSH 2
    STORE
    JMP end
end:
    HALT
```
```
a = 1
while(a < 5) {
    b = b + 1;
}
```

```as

entry:
    PUSH 0x100
    PUSH 1
    STORE
    PUSH 5 
    JMP bb0
bb0:
    PUSH 0x100
    LOAD // top 1
    LW // 1
    JE bb1 end
bb1:

    -
    -
    -

    JMP bb0 

end:
    HALT
```

```cpp
visit(AssignNode node)  {
    std::visit(node->left);
    std::visit(node->right);
    generate("STORE");
}

visit(Number node)  {
    generate("PUSH" + std::to_string(node.value));
}

visit(IdentNode node)  {
    generate("PUSH" + std::to_string(0x100));
}
```
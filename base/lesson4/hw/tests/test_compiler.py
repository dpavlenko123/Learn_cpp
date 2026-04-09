import pytest
import subprocess
import tempfile
import os
from pathlib import Path

TEST_DIR = Path(__file__).parent / "fixtures"

def test_all():
    for spl_file in TEST_DIR.glob("*.spl"):
        expected_file = spl_file.with_suffix(".expected")
        if not expected_file.exists():
            continue
        
        with tempfile.NamedTemporaryFile(suffix='.asm', delete=False) as tmp:
            output_file = tmp.name
        
        try:
            compiler_path = "../build/compiler"
            if not os.path.exists(compiler_path):
                compiler_path = "./compiler"
            
            result = subprocess.run([compiler_path, str(spl_file), output_file], 
                                   capture_output=True)
            
            with open(output_file) as f:
                generated = f.read()
            with open(expected_file) as f:
                expected = f.read()
            
            assert generated.strip() == expected.strip(), f"Failed: {spl_file.name}"
        finally:
            os.unlink(output_file)
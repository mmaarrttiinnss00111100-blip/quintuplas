name: Build NDS
on: [push, workflow_dispatch]
jobs:
  build:
    runs-on: ubuntu-latest
    container: devkitpro/devkitarm
    steps:
      - uses: actions/checkout@v4
      - run: dkp-pacman -S --noconfirm --needed nds-dev
      - name: Build
        run: |
          set -e
          EX=/opt/devkitpro/examples/nds
          ls $EX | head -40
          T=$(find $EX -path '*hello_world/Makefile' | head -1)
          if [ -z "$T" ]; then T=$(grep -rl 'lnds9' $EX --include=Makefile | head -1); fi
          echo "template: $T"
          rm -rf /tmp/quintuplas
          mkdir -p /tmp/quintuplas
          cp -r "$(dirname "$T")"/. /tmp/quintuplas/
          rm -rf /tmp/quintuplas/source
          mkdir -p /tmp/quintuplas/source
          cp main.c /tmp/quintuplas/source/main.c
          cd /tmp/quintuplas
          sed -i 's/\\n Conquista: %s\\n\\n /\\n\\n ~ %s ~\\n\\n /' source/main.c
          sed -i 's/^\(LIBS[[:space:]]*:=\)/\1 -lfat/' Makefile
          grep -n '^LIBS' Makefile
          find /opt/devkitpro -name 'libfat.a'
          make
          N=$(find /tmp/quintuplas -maxdepth 1 -name '*.nds' | head -1)
          echo "nds: $N"
          cp "$N" "$GITHUB_WORKSPACE/quintuplas.nds"
      - uses: actions/upload-artifact@v4
        with:
          name: quintuplas-nds
          path: quintuplas.nds

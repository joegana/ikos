; ModuleID = 'bool.cpp.pp.bc'
source_filename = "bool.cpp"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.12.0"

; CHECK-LABEL: Bundle
; CHECK: target-endianness = little-endian
; CHECK: target-pointer-size = 64 bits
; CHECK: target-triple = x86_64-apple-macosx10.12.0

@b = global i8 1, align 1, !dbg !0
; CHECK: define ui8* @b, align 1, init {
; CHECK: #1 !entry !exit {
; CHECK:   store @b, 1, align 1
; CHECK: }
; CHECK: }

; Function Attrs: noinline nounwind ssp uwtable
define i32 @_Z1fb(i1 zeroext) #0 !dbg !11 {
  %2 = alloca i32, align 4
  %3 = alloca i8, align 1
  %4 = zext i1 %0 to i8
  store i8 %4, i8* %3, align 1
  call void @llvm.dbg.declare(metadata i8* %3, metadata !15, metadata !16), !dbg !17
  call void @llvm.trap(), !dbg !18
  unreachable, !dbg !18
                                                  ; No predecessors!
  %6 = load i32, i32* %2, align 4, !dbg !19
  ret i32 %6, !dbg !19
}
; CHECK: define si32 @_Z1fb(ui1 %1) {
; CHECK: #1 !entry !unreachable {
; CHECK:   si32* $2 = allocate si32, 1, align 4
; CHECK:   ui8* $3 = allocate ui8, 1, align 1
; CHECK:   ui8 %4 = zext %1
; CHECK:   store $3, %4, align 1
; CHECK:   call @ar.trap()
; CHECK:   unreachable
; CHECK: }
; CHECK: }

; Function Attrs: nounwind readnone
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: noreturn nounwind
declare void @llvm.trap() #2
; CHECK: declare void @ar.trap()

; Function Attrs: noinline norecurse nounwind ssp uwtable
define i32 @main() #3 !dbg !21 {
  %1 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  ret i32 0, !dbg !24
}
; CHECK: define si32 @main() {
; CHECK: #1 !entry !exit {
; CHECK:   si32* $1 = allocate si32, 1, align 4
; CHECK:   store $1, 0, align 4
; CHECK:   return 0
; CHECK: }
; CHECK: }

attributes #0 = { noinline nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone }
attributes #2 = { noreturn nounwind }
attributes #3 = { noinline norecurse nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!7, !8, !9}
!llvm.ident = !{!10}

!0 = !DIGlobalVariableExpression(var: !1)
!1 = distinct !DIGlobalVariable(name: "b", scope: !2, file: !3, line: 1, type: !6, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus, file: !3, producer: "clang version 4.0.1 (tags/RELEASE_401/final)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, globals: !5)
!3 = !DIFile(filename: "bool.cpp", directory: "/Users/marthaud/ikos/ikos-git/frontend/llvm/test/regression/import/no_optimization")
!4 = !{}
!5 = !{!0}
!6 = !DIBasicType(name: "bool", size: 8, encoding: DW_ATE_boolean)
!7 = !{i32 2, !"Dwarf Version", i32 4}
!8 = !{i32 2, !"Debug Info Version", i32 3}
!9 = !{i32 1, !"PIC Level", i32 2}
!10 = !{!"clang version 4.0.1 (tags/RELEASE_401/final)"}
!11 = distinct !DISubprogram(name: "f", linkageName: "_Z1fb", scope: !3, file: !3, line: 3, type: !12, isLocal: false, isDefinition: true, scopeLine: 3, flags: DIFlagPrototyped, isOptimized: false, unit: !2, variables: !4)
!12 = !DISubroutineType(types: !13)
!13 = !{!14, !6}
!14 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!15 = !DILocalVariable(name: "i", arg: 1, scope: !11, file: !3, line: 3, type: !6)
!16 = !DIExpression()
!17 = !DILocation(line: 3, column: 12, scope: !11)
!18 = !DILocation(line: 3, column: 15, scope: !11)
!19 = !DILocation(line: 3, column: 16, scope: !20)
!20 = !DILexicalBlockFile(scope: !11, file: !3, discriminator: 1)
!21 = distinct !DISubprogram(name: "main", scope: !3, file: !3, line: 5, type: !22, isLocal: false, isDefinition: true, scopeLine: 5, flags: DIFlagPrototyped, isOptimized: false, unit: !2, variables: !4)
!22 = !DISubroutineType(types: !23)
!23 = !{!14}
!24 = !DILocation(line: 6, column: 3, scope: !21)
